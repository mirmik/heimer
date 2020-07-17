#ifndef HEIMER_AXIS_H
#define HEIMER_AXIS_H

struct axis_node_s 
{
	control_node_t node;

	float feedpos;
	float feedspd;

	float ctrpos;
	float ctrspd;
};

class virtual_axis_s 
{
	struct axis_node_s ax;
	control_node_t * parent;
};

__BEGIN_DECLS

static inline
void axis_node_init(struct axis_node_s * axis,
	const char * name,
	const struct control_ops * c_ops,
	int flags) 
{
	axis -> feedpos = 0;
	axis -> feedspd = 0;
	axis -> ctrpos = 0;
	axis -> ctrspd = 0;

	control_node_init(&axis->node,
		name,
		c_ops,  //constrol
		flags
	);
}

/*int virtual_axis_take_external_control(control_node_t * dev, control_node_t * master) 
{
	virtual_axis_t * vax = mcast_out(dev, virtual_axis_t, ax);
}*/

static inline
void virtual_axis_init(
	struct virtual_axis_s * vaxis, 
	const char* name) 
{
	axis_node_init(
		&vaxis->ax, 
		name, 
		NULL,
		NULL,
		NULL);
}

__END_DECLS

#endif