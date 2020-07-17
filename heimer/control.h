#ifndef HEIMER_CONTROL_H
#define HEIMER_CONTROL_H

#include <igris/datastruct/dlist.h>

#define HEIM_ERR_IN_OPERATE  (-1)
#define HEIM_ERR_IS_BUSY     (-2)
#define HEIM_ERR_IS_NOACTIVE (-3)
#define HEIM_ERR_IS_CONTROL_FAULT (-4)
#define HEIM_ERR_IS_ALARM (-5)

#define HEIM_IS_ACTIVE      (1<<0)
#define HEIM_ALARM          (1<<1)
#define HEIM_MASTER         (1<<2)
#define HEIM_SLAVE          (1<<3)

extern struct dlist_head heimer_control_nodes;

struct control_node;
typedef struct control_node control_node_t;

struct control_ops 
{
	// Вызывается при попытке взять активировать устройство
	//int (*activate)(control_node_t * node);

	// Вызывается при после успешной активации устройства
	int (*activate)(control_node_t * node);

	// Вызывается при попытке деактивировать устройство
	int (*deactivate)(control_node_t * node);

	// вызывается при взятии внешнего управления нодом
	int (*on_take) (
		control_node_t * slave, 
		control_node_t * master);
	
	// вызывается при отпускании внешнего управления
	int (*on_release) (
		control_node_t * slave, 
		control_node_t * master);

	// обратное уведомления о событиях
	int (*interrupt) (
		control_node_t * master, 
		control_node_t * slave, // подчинённый, переславший сигнал
		control_node_t * source, // источник сигнала
		int code, 
		int subcode);
	
	// итератор подчинённых устройств
	int (*iterate) (control_node_t * master, control_node_t * it);
};

struct control_node 
{
	struct dlist_head lnk;

	const struct master_ops   * m_ops;
	const struct slave_ops    * s_ops;
	const struct control_ops  * c_ops;

	struct control_node * extctr; // внешний контроллер
	struct control_node * parent; // для составных устройств
	int flags;
};

__BEGIN_DECLS

static inline 
void control_node_init(
	struct control_node       * node,
	const struct master_ops   * m_ops,
	const struct slave_ops    * s_ops,
	const struct control_ops  * c_ops) 
{
	node -> m_ops = m_ops;
	node -> s_ops = s_ops;
	node -> c_ops = c_ops;

	node -> parent = NULL;
	node -> extctr = NULL;

	node -> flags = 0;
}

static inline
int heimer_activate(control_node_t * node) 
{
	int sts;

	if (node->flags & HEIM_IS_ACTIVE)
		return 0;

	if (node->m_ops && (sts = heimer_take_control(master))) 
		return sts; 

	if (node->c_ops->activate && (sts = node->c_ops->activate(node)))
	{
		heimer_release_control(master); 
		return sts;
	}
	
	node->flags |= HEIM_IS_ACTIVE;
	node->c_ops->on_activate(node);
	return 0;
}

static inline
int heimer_deactivate(control_node_t * node) 
{
	if (!(node->flags & HEIM_IS_ACTIVE))
		return 0;

	if (node->c_ops->deactivate && (sts = node->c_ops->deactivate(node))) 
		return sts;

	if (node->m_ops) 
		heimer_release_control(node); 

	node->flags &= ~HEIM_IS_ACTIVE;
	return 0;
}

static inline
void heimer_release_control(control_node_t * master, control_node_t * slave) 
{
	int sts = 0;

	control_node_t* it = NULL;
	while ((it = master->iterate(master, it)))
	{
		if (it->extctr == master) 
		{
			sts = it->external_release(master, it);
			it->extctr = NULL;
		}
	}

	(void) sts;
	return 0;
}

static inline
int heimer_take_external_control(
	control_node_t * slave, 
	control_node_t * master) 
{
	int sts;

	assert(master->flags & HEIM_MASTER);
	assert(slave->flags & HEIM_SLAVE);

	if (slave->flags & HEIM_IS_ALARM)
		return HEIM_ERR_IS_ALARM;

	if (!(slave->flags & HEIM_IS_ACTIVE)) 
		return HEIM_ERR_IS_NOACTIVE;

	if (slave->c_ops->external_take, 
		&& (sts = slave->s_ops->external_take(slave, master))
	) 
		return sts;

	slave->extctr = master;
	return 0;
} 


static inline
int heimer_take_control(control_node_t * master) 
{
	int sts = 0;

	assert(master->flags & HEIM_MASTER);

	control_node_t* it = NULL;
	while ((it = master->m_ops->iterate(master, it)))
	{
		if (sts = heimer_take_external_control(it, master))
		{
			heimer_release_control(master);
			break;
		}
	}

	return 0;
}

__END_DECLS

#endif