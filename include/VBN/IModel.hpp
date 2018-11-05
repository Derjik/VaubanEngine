#ifndef I_MODEL_HPP_INCLUDED
#define I_MODEL_HPP_INCLUDED

#include <memory>
#include <SDL2/SDL_types.h>

class EngineUpdate;

class IModel
{
	public:
		virtual void elapse(Uint32 const gameTicks,
			std::shared_ptr<EngineUpdate> engineUpdate) = 0;
};

#endif // I_MODEL_HPP_INCLUDED
