// Объект, управляющей позицией через скорость.
// Phaser работает в ускоренном контексте.

// Позиция целочисленная, поскольку датчики работают в целых числах.

struct phaser_s 
{
	float delta;
	float ctrspd;
	float gain;

	int64_t tgtpos;  // модельная позиция.
	int64_t feedpos; // измеренная или расщетная позиция.
};

__BEGIN_DECLS

static inline 
void phaser_init(
	struct phaser_s * phaser,
	float delta,
	float gain) 
{
	phaser->delta = delta;
	phaser->gain = gain;

	phaser->ctrspd = 0;
}

__END_DECLS