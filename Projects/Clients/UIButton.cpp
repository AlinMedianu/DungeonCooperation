#include "UIButton.h"

namespace UI
{
	Button::Button(Sprite::Sheet& body, sf::Text& message)
		: body(body), message(message), bounds{}, current{}, clicked{}
	{

	}

	void Button::Update(const sf::Window& on)
	{
		if (current == State::Inactive)
			return;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (current != State::Down && bounds.contains(sf::Vector2f{ sf::Mouse::getPosition(on) }))
			{
				current = State::Down;
				body.ChangeTo(static_cast<size_t>(current));
			}
		}
		else
		{
			if (bounds.contains(sf::Vector2f{ sf::Mouse::getPosition(on) }))
			{
				if (current == State::Down)
					clicked = true;
				if (current != State::Hover)
				{
					current = State::Hover;
					body.ChangeTo(static_cast<size_t>(current));
				}
			}
			else if (current != State::Default)
			{
				current = State::Default;
				body.ChangeTo(static_cast<size_t>(current));
			}
		}
	}

	void Button::Activate()
	{
		if (current == State::Inactive)
		{
			current = State::Default;
			body.ChangeTo(static_cast<size_t>(current));
		}
	}

	void Button::Deactivate()
	{
		if (current != State::Inactive)
		{
			current = State::Inactive;
			body.ChangeTo(static_cast<size_t>(current));
		}
	}

	void Button::Move(sf::Vector2f position)
	{
		body.Move(position);
		message.move(position);
	}

	void Button::SetScale(float factor)
	{
		body.SetScale(factor);
		message.setScale(factor, -factor);
	}

	void Button::SetOrigin(float localX, float localY)
	{
		assert(localX >= 0 && localX <= 1 && localY >= 0 && localY <= 1);
		body.SetOrigin(localX, localY);
		message.setOrigin(message.getLocalBounds().left + message.getLocalBounds().width * localX,
			message.getLocalBounds().top + message.getLocalBounds().height * localY);
	}

	void Button::RecalculateBounds(const Renderer& renderer)
	{
		bounds = renderer.WorldCoordinateToScreenPosition(body.GetGlobalBounds());
	}

	bool Button::WasClicked() noexcept
	{
		bool wasClicked = clicked;
		clicked = false;
		return wasClicked;
	}

	Button::State Button::GetCurrentState() const noexcept
	{
		return current;
	}

	void Button::GetDrawn(Renderer& by) const
	{
		body.GetDrawn(by);
		by.Draw(message);
	}
}
