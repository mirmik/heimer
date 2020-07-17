#ifndef HEIMER_PHASER_AXIS_H
#define HEIMER_PHASER_AXIS_H

struct phaser_axis_s
{
	axis_node_s ax;

	struct phaser_s * phaser;
	float poskoeff;
};

__BEGIN_DECLS

static inline
void phaser_axis_serve(control_node_t * node) 
{
	struct phaser_axis_s * pax = mcast_out(
		node, struct phaser_axis_s, ax->node);

	float curpos = pax->ax->feedpos;
	float diff = pax->ax->ctrpos - curpos;

	float evalspeed = pax->ax->ctrspd + pax->poskoeff * diff;
	
	phaser_set_control(evalspeed);
}

static inline
void phaser_axis_feedback(control_node_t * node) 
{
	struct phaser_axis_s * pax = mcast_out(
		node, struct phaser_axis_s, ax->node);

	pax->ax->feedpos = phaser_current_position(pax->phaser); 
	pax->ax->feedspd = phaser_current_speed(pax->phaser);
}

__END_DECLS

#endif