#pragma once
#include "../ECS.h"

enum class Tag
{
    None,
    Player,
    Enemy
};

struct TagComponent : Component
{
    TagComponent(Tag tag = Tag::None) : mTag(tag) {}
    
    Tag GetTag() const {return mTag;}
    void SetTag(Tag tag) {mTag = tag;}
    bool IsTag(Tag tag) const {return mTag == tag;}
    bool CompareTag(const TagComponent& other) const {return mTag == other.mTag;}

private:
    Tag mTag;
};