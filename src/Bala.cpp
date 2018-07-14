#include "Bala.h"
#include "Animation.h"

#include <stdio.h>

using namespace std;
Animation *animationBullet;
Animation *animationBoomerang;

Bala::Bala()
{
}

void Bala::setup(string path, bool f_p, ofVec2f _position, float s, char tipo)
{
	_tipo = tipo;
	fromPlayer = f_p;

	if (_tipo == A)
	{
		position.set(_position.x, _position.y + 8);
	}

	if (_tipo == B)
	{
		position.set(_position.x, _position.y - 8);
	}

	if (_tipo == G)
	{
		position.set(_position.x, _position.y + 8);
	}

	speed = s + 3;

	if (_tipo != G)
	{
		sprite.load(path);
	}
	else
	{
		vai = true;
		positionInitBoo.set(_position.x, _position.y + 8);
		animationBoomerang = new Animation();
		animationBoomerang->setup(path, 8, 8);
	}

	width = sprite.getWidth();

	momentum.set(0, 0);
	mass = 10;
	gravidade.set(0, 60);

	if (_tipo != G)
	{
		animationBullet = new Animation();
		animationBullet->setup("EXPLOSION", 8, 8);
	}
}

void Bala::update(float deltaTime, ofVec2f vecmouseGet)
{

	if (_tipo == A)
	{
		ofVec2f forces;
		ofVec2f acceleration;
		ofVec2f accelSecs;

		forces += vecmouseGet.normalize() * speed;
		acceleration = forces / mass;

		accelSecs = acceleration * deltaTime;
		position += (momentum + accelSecs) * deltaTime;
		momentum += accelSecs;
	}

	//cout << _tipo << endl;
	if (_tipo == B)
	{
		ofVec2f forces;
		ofVec2f acceleration;
		ofVec2f accelSecs;

		forces = forces * speed;

		acceleration = forces / mass;
		accelSecs = (acceleration + gravidade) * deltaTime;

		position += (momentum + accelSecs) * deltaTime;
		momentum += accelSecs;
	}

	if (_tipo == E)
	{
		momentum.y = 0;
	}

	if (_tipo == G)
	{
		ofVec2f forces;
		ofVec2f acceleration;
		ofVec2f accelSecs;

		forces += vecmouseGet.normalize() * speed;
		acceleration = forces / mass;

		accelSecs = acceleration * deltaTime;
		position += (momentum + accelSecs) * deltaTime;
		momentum += accelSecs;

		if (!volta && vai)
		{
			if (position.x >= positionInitBoo.x + 100 || position.x <= positionInitBoo.x - 100)
			{
				volta = true;
				vai = false;
			}
		}

		if (volta && !vai)
		{
			momentum.x = momentum.x * -1;
			vai = true;
		}
	}
}

void Bala::draw()
{

	if (_tipo == A || _tipo == B)
	{
		ofTranslate(position.x, position.y);
		sprite.draw(0, 0);
	}

	if (_tipo == E)
	{
		if (tryOnemoreTime)
		{
			sprite.clear();
		}

		animationBullet->draw(position.x - 32, position.y - 32);
		tryOnemoreTime = false;

		//cout << animationBullet->imageno << endl;
		if (animationBullet->imageno == 7)
		{
			paraAnimation = true;
		}
	}

	if (_tipo == G)
	{
		animationBoomerang->draw(position.x, position.y);
	}
}

void Bala::impulso(bool heroLeft, float angleCanhao)
{
	if (_tipo == A)
	{
		momentum.set(0, 0);
		ofVec2f impulse;
		impulse.set(3000, 0);
		if (heroLeft)
		{
			impulse.set(-3000, 0);
		}

		momentum += impulse / mass;
	}

	if (_tipo == B)
	{
		momentum.set(0, 0);
		ofVec2f impulse;
		impulse.set(1000 * cos(angleCanhao * PI / 180), 1000 * sin(angleCanhao * PI / 180));
		momentum += impulse / mass;
	}

	if (_tipo == G)
	{
		momentum.set(0, 0);
		ofVec2f impulse;
		impulse.set(900, 0);
		if (heroLeft)
		{
			impulse.set(-900, 0);
		}

		momentum += impulse / mass;
	}
}

bool Bala::colidiu(int tile)
{
	//colide com bordas

	if (ofGetWidth() + position.x < 0 || position.x + width > ofGetWindowWidth())
		return true;

	if (position.y > ofGetWindowHeight())
		return true;

	if (tile == 16 || tile == 18)
		return true;

	return false;
}
bool Bala::colidiuExplosao(int width, int height, ofVec2f outraPosicao, int minhaWidth, int minhaHeight)
{
	//colidir com heroi ou enemy

	if (position.x < outraPosicao.x + width &&
			outraPosicao.x < position.x + minhaWidth &&
			position.y < outraPosicao.y + height &&
			outraPosicao.y < position.y + minhaHeight)
	{
		return true;
	}

	return false;
}

Bala::~Bala()
{
}
