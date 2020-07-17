#ifndef HEIMER_AXIS_H
#define HEIMER_AXIS_H

// Интерфейсный драйвер для управления одной осью.

#include <heimer/control.h>

struct oneaxis_params 
{
	float default_spd;
	float default_acc;
	float default_dcc;
};

struct oneaxis_s
{
	struct control_node node;
	struct dlist_head axes_list;

	float spd;
	float acc;
	float dcc;

	float maxspd;
	float maxacc;
	float maxdcc;

	axis_node_t * controlled;
};
typedef struct oneaxis_s oneaxis_t;

__BEGIN_DECLS

static inline 
void oneaxis_params_init(struct oneaxis_params * params) 
{
	params->defspd = 1.0;
	params->defacc = 1.0;
	params->defdcc = 1.0;

	params->maxspd = 1.0;
	params->maxacc = 1.0;
	params->maxdcc = 1.0;
} 

static inline 
void oneaxis_init(oneaxis_t * axis, 
	const struct axis_params * params) 
{
	axis->spd = params->defspd;
	axis->acc = params->defacc;
	axis->dcc = params->defdcc;

	axis->maxspd = params->maxspd;
	axis->maxacc = params->maxacc;
	axis->maxdcc = params->maxdcc;
}

__END_DECLS

#endif