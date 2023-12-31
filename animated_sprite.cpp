#include "animated_sprite.h"

animated_sprite::animated_sprite() = default;
animated_sprite::animated_sprite(const sf::Texture& texture, const int num_frames, const float frame_duration)
    : sprite_(texture), num_frames_(num_frames), current_frame_(0), frame_duration_(frame_duration), elapsed_(0.0f)
{
    // Calculate the width of each frame
    frame_width_ = static_cast<int>(texture.getSize().x) / num_frames;
    sprite_.setTextureRect(sf::IntRect(0, 0, frame_width_, static_cast<int>(texture.getSize().y)));
}

void animated_sprite::set_position(const sf::Vector3f& position)
{
    sprite_.setScale(position.z, position.z);
    sprite_.setPosition(position.x, position.y);
}

void animated_sprite::update(const float dt)
{
    elapsed_ += dt;
    if (elapsed_ >= frame_duration_)
    {
        // Move to the next frame
        current_frame_ = (current_frame_ + 1) % num_frames_;

        // Update the texture rectangle to show the current frame
        sprite_.setTextureRect(
            sf::IntRect(current_frame_ * frame_width_, 0, frame_width_, sprite_.getTextureRect().height));

        elapsed_ = 0.0f; // Reset the timer
    }
}

sf::Sprite animated_sprite::draw()
{
    return sprite_;
}

    

