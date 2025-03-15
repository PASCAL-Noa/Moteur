#pragma once

class IResource
{
public:
    virtual ~IResource() = default;
    virtual void Load(const std::string& path) = 0;
    
};