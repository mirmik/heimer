#ifndef HEIMER_AXIS_H
#define HEIMER_AXIS_H

#include <heimer/axis.h>

struct axis_node_s 
{
	control_node_t node;

	float feedpos;
	float feedspd;

	float ctrpos;
	float ctrspd;
};
typedef struct axis_node_s axis_node_t;

struct virtual_axis_s 
{
	struct axis_node_s ax;
	control_node_t * parent;
};
typedef struct virtual_axis_s virtual_axis_t; 

__BEGIN_DECLS

__END_DECLS

#endif