#ifndef HEIMER_PAIR_H
#define HEIMER_PAIR_H

struct paired_axes 
{
	virtual_axis_t actr;
	virtual_axis_t bctr;

	virtual_axis_t * a_controlled;
	virtual_axis_t * b_controlled;

	float  aa,  ab,  ba,  bb;
	float iaa, iab, iba, ibb;
};

__BEGIN_DECLS

void paired_axes_init(
	struct paired_axes * pair,
	virtual_axis_t * a_controlled,
	virtual_axis_t * b_controlled,

	float aa, float ab, float ba, float bb
) {
	pair -> a_controlled = a_controlled;
	pair -> b_controlled = b_controlled;

	pair -> aa = aa;
	pair -> ab = ab;
	pair -> ba = ba;
	pair -> bb = bb;

	float denum = (a*d - b*c);

	pair -> iaa = bb / denum;
	pair -> iab = ba / denum;
	pair -> iba = ab / denum;
	pair -> ibb = aa / denum;	
}

void paired_axes_feedback(struct paired_axes * pair) 
{
	float afeedpos = pair->a_controlled->feedpos;
	float bfeedpos = pair->b_controlled->feedpos;
	float afeedspd = pair->a_controlled->feedspd;
	float bfeedspd = pair->b_controlled->feedspd;

	actr->feedpos = afeedpos - pair->ba_koeff * bfeedpos;
};

void paired_axes_serve(struct paired_axes * pair) 
{
};

__END_DECLS

#endif