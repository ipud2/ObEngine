//Author : Sygmei
//Key : 976938ef7d46c286a2027d73f3a99467bcfa8ff0c1e10bd0016139744ef5404f4eb4d069709f9831f6de74a094944bf0f1c5bf89109e9855290336a66420376f

#include "SpriteFunc.hpp"

namespace obe
{
	namespace Graphics
	{
		sf::Sprite getCleanSprite(sf::Texture texture)
		{
			sf::Sprite returnSprite;
			returnSprite.setTexture(texture);
			return returnSprite;
		}
	}
}