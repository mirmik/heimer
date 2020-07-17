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


__END_DECLS

#endif