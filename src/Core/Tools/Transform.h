#pragma once
#include <DirectXMath.h>

struct Transform
{

    //================================
    //          Scale
    //================================
    DirectX::XMFLOAT3 vSca;
    DirectX::XMFLOAT4X4 mSca;

    //================================
    //          Rotation
    //================================
    DirectX::XMFLOAT3 vDir;
    DirectX::XMFLOAT3 vRight;
    DirectX::XMFLOAT3 vUp;
    DirectX::XMFLOAT4 qRot; // Rotation en Quaternion
    DirectX::XMFLOAT4X4 mRot;
    float mPhi = DirectX::XM_PIDIV2;
    float mTheta = 0;

    //================================
    //          Position
    //================================
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT4X4 mPos;

    //================================
    //          Matrix
    //================================
    DirectX::XMFLOAT4X4 matrix;

    //================================
    //          Fonctions
    //================================

    Transform();

    void Identity();
    static void SetIdentity(DirectX::XMFLOAT4X4& matrix);
    
    void UpdateMatrix();
    
    void AddRotate(float yaw, float pitch, float roll);
    void SetRotation(float yaw, float pitch, float roll);
    void RotateQuaternion(const DirectX::XMFLOAT4& quat);
    void SlerpRotation(const DirectX::XMFLOAT4& targetQuat, float t); // Rotation fluide
    
    void SetPosition(float x, float y, float z);
    void LerpPosition(const DirectX::XMFLOAT3& target, float t); // Equivalent d'un GoToPosition, mouvement fluide
    
    void Translate(float x, float y, float z);
    
    void SetScale(float x, float y, float z);
    
    void LookAt(const DirectX::XMFLOAT3& target);

    //================================
    //          Getters
    //================================
    DirectX::XMMATRIX GetMatrix() const {return XMLoadFloat4x4(&matrix);}

    DirectX::XMFLOAT3 GetForward() const {return vDir;}
    DirectX::XMFLOAT3 GetRight() const {return vRight;}
    DirectX::XMFLOAT3 GetUp() const {return vUp;}

    DirectX::XMFLOAT3 GetPosition() const {return position;};
    DirectX::XMFLOAT3 GetScale() const {return vSca;}
    DirectX::XMFLOAT4 GetRotation() const {return qRot;}
    DirectX::XMVECTOR GetPositionVector() const;

    const float& GetPhi()const { return mPhi; }
    const float& GetTheta()const { return mTheta; }
    void SetPhi(float phi) { mPhi = phi; }
    void SetTheta(float theta) { mTheta = theta; }
};
