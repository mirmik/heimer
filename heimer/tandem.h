#ifndef HEIMER_TANDEM_H
#define HEIMER_TANDEM_H

// Оптимизированная для минимизации вычислений пара осей.
// Взаимодействие осей задаётся одни коэффициентом.

struct paired_axes 
{
	control_node_t node;

	virtual_axis_t master;
	virtual_axis_t slave;

	virtual_axis_t * master_controlled;
	virtual_axis_t * slave_controlled;

	float k;
};

__BEGIN_DECLS

static void _on_activate(control_node_t * node) 
{
	struct paired_axes * pair = mcast_out(node, struct paired_axes, node);
	paired_axes_feedback(pair);

	master->ctrpos = master->feedpos;
	master->ctrspd = 0;

	slave->ctrpos  = slave->feedpos;
	slave->ctrspd  = 0;
}

static control_node_t * iterate(control_node_t * self, control_node_t * it) 
{
	struct paired_axes * pair = mcast_out(node, struct paired_axes, node);

	if (it == NULL) 
		return pair->master_controlled;
	else if (it == pair->master_controlled) 
		return pair->slave_controlled;
	
	return NULL;
}

const struct control_ops paired_axes_operations = 
{
	.activate = NULL,
	.deactivate = NULL,
	.on_activate = _on_activate,
	.iterate = _iterate,
	.feedback = paired_axes_feedback,
	.serve = paired_axes_serve
};

void paired_axes_init(
	struct paired_axes * pair,
	virtual_axis_t * master_controlled,
	virtual_axis_t * slave_controlled,

	float k
) {
	pair -> master_controlled = master_controlled;
	pair -> slave_controlled = slave_controlled;

	pair -> k = k;	
}

void paired_axes_feedback(struct paired_axes * pair) 
{
	float mx = pair->master_controlled->feedpos;
	float mv = pair->master_controlled->feedspd;

	float sx = pair->slave_controlled->feedpos;
	float sv = pair->slave_controlled->feedspd;

	master->feedpos = mx;
	master->feedspd = mv;

	slave->feedpos = sx - pair->k * mx;
	slave->feedspd = sv - pair->k * mv;
};

void paired_axes_serve(struct paired_axes * pair) 
{
	float mx = pair->master_controlled->ctrpos;
	float mv = pair->master_controlled->ctrspd;

	float sx = pair->slave_controlled->ctrpos;
	float sv = pair->slave_controlled->ctrspd;

	master->ctrpos = mx;
	master->ctrspd = mv;

	slave->ctrpos = sx + pair->k * mx;
	slave->ctrspd = sv + pair->k * mv;
};	

__END_DECLS

#endif