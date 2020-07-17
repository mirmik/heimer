#include <heimer/control.h>

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

int heimer_activate(control_node_t * node) 
{
	int sts;

	if (node->flags & HEIM_IS_ACTIVE)
		return 0;

	if (node->m_ops && (sts = heimer_take_control(node))) 
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
