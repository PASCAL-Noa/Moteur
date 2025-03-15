#include "pch.h"
#include "Transform.h"

Transform::Transform()
{
    Identity();
}

void Transform::SetIdentity(DirectX::XMFLOAT4X4& matrix)
{
    matrix = DirectX::XMFLOAT4X4
    (
         1.0f, 0.0f, 0.0f, 0.0f,
         0.0f, 1.0f, 0.0f, 0.0f,
         0.0f, 0.0f, 1.0f, 0.0f,
         0.0f, 0.0f, 0.0f, 1.0f
     );
}

void Transform::Identity()
{
    vSca = { 1.0f, 1.0f, 1.0f };
    SetIdentity(mSca);
    
    vDir = { 0.0f, 0.0f, 1.0f };
    vRight = { 1.0f, 0.0f, 0.0f };
    vUp = { 0.0f, 1.0f, 0.0f };
    qRot = { 0.0f, 0.0f, 0.0f, 1.0f };
    SetIdentity(mRot);
    
    position = { 0.0f, 0.0f, 0.0f };
    SetIdentity(mPos);

    UpdateMatrix();
}

void Transform::UpdateMatrix()
{
    // Création des matrices de transformation
    DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(vSca.x, vSca.y, vSca.z);
    DirectX::XMMATRIX rotationMatrix = XMLoadFloat4x4(&mRot);
    DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

    // Produit final : S * R * T
    DirectX::XMMATRIX finalMatrix = scaleMatrix * rotationMatrix * translationMatrix;
    XMStoreFloat4x4(&matrix, finalMatrix);

    DirectX::XMFLOAT4X4 rotMatrix;
    XMStoreFloat4x4(&rotMatrix, rotationMatrix);

    vRight = { rotMatrix._11, rotMatrix._12, rotMatrix._13 }; // Première colonne
    vUp = { rotMatrix._21, rotMatrix._22, rotMatrix._23 }; // Deuxième colonne
    vDir = { rotMatrix._31, rotMatrix._32, rotMatrix._33 }; // Troisième colonne
}


void Transform::AddRotate(float yaw, float pitch, float roll)
{
    DirectX::XMVECTOR qCurrentRot = XMLoadFloat4(&qRot);

    DirectX::XMVECTOR qPitch = DirectX::XMQuaternionRotationAxis(XMLoadFloat3(&vRight), pitch);
    DirectX::XMVECTOR qYaw = DirectX::XMQuaternionRotationAxis(XMLoadFloat3(&vUp), yaw);
    DirectX::XMVECTOR qRoll = DirectX::XMQuaternionRotationAxis(XMLoadFloat3(&vDir), roll);

    // Appliquer les rotations dans l'ordre Roll -> Pitch -> Yaw
    DirectX::XMVECTOR qFinalRot = DirectX::XMQuaternionMultiply(qRoll, DirectX::XMQuaternionMultiply(qPitch, qYaw));

    qFinalRot = DirectX::XMQuaternionMultiply(qFinalRot, qCurrentRot);
    qFinalRot = DirectX::XMQuaternionNormalize(qFinalRot);

    XMStoreFloat4(&qRot, qFinalRot);

    DirectX::XMMATRIX mRotationMatrix = DirectX::XMMatrixRotationQuaternion(qFinalRot);
    XMStoreFloat4x4(&mRot, mRotationMatrix);

    vRight = { mRotationMatrix.r[0].m128_f32[0], mRotationMatrix.r[0].m128_f32[1], mRotationMatrix.r[0].m128_f32[2] };
    vUp = { mRotationMatrix.r[1].m128_f32[0], mRotationMatrix.r[1].m128_f32[1], mRotationMatrix.r[1].m128_f32[2] };
    vDir = { mRotationMatrix.r[2].m128_f32[0], mRotationMatrix.r[2].m128_f32[1], mRotationMatrix.r[2].m128_f32[2] };

    UpdateMatrix();
}

void Transform::SetRotation(float yaw, float pitch, float roll)
{
   
    // Créer les quaternions pour chaque axe
    DirectX::XMVECTOR qPitch = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), pitch); // Axe X
    DirectX::XMVECTOR qYaw = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), yaw);   // Axe Y
    DirectX::XMVECTOR qRoll = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), roll); // Axe Z

    // Combiner les rotations dans l'ordre Roll -> Pitch -> Yaw
    DirectX::XMVECTOR qFinalRot = DirectX::XMQuaternionMultiply(qRoll, DirectX::XMQuaternionMultiply(qPitch, qYaw));

    // Normaliser le quaternion
    qFinalRot = DirectX::XMQuaternionNormalize(qFinalRot);

    // Stocker la nouvelle rotation
    XMStoreFloat4(&qRot, qFinalRot);

    // Convertir le quaternion en matrice de rotation
    DirectX::XMMATRIX mRotationMatrix = DirectX::XMMatrixRotationQuaternion(qFinalRot);
    XMStoreFloat4x4(&mRot, mRotationMatrix);

    // Mettre à jour les vecteurs locaux
    vRight = { mRotationMatrix.r[0].m128_f32[0], mRotationMatrix.r[0].m128_f32[1], mRotationMatrix.r[0].m128_f32[2] };
    vUp = { mRotationMatrix.r[1].m128_f32[0], mRotationMatrix.r[1].m128_f32[1], mRotationMatrix.r[1].m128_f32[2] };
    vDir = { mRotationMatrix.r[2].m128_f32[0], mRotationMatrix.r[2].m128_f32[1], mRotationMatrix.r[2].m128_f32[2] };

   
    UpdateMatrix();

}


void Transform::RotateQuaternion(const DirectX::XMFLOAT4& quat)
{
    DirectX::XMVECTOR qNewRot = XMLoadFloat4(&quat);
    qNewRot = DirectX::XMQuaternionNormalize(qNewRot); // Normalisation pour éviter les erreurs

    XMStoreFloat4(&qRot, qNewRot);
    XMStoreFloat4x4(&mRot, DirectX::XMMatrixRotationQuaternion(qNewRot));

    UpdateMatrix();
}

void Transform::SlerpRotation(const DirectX::XMFLOAT4& targetQuat, float t)
{
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    DirectX::XMVECTOR qStart = DirectX::XMLoadFloat4(&qRot);
    DirectX::XMVECTOR qEnd = DirectX::XMLoadFloat4(&targetQuat);
    
    DirectX::XMVECTOR qInterpolated = DirectX::XMQuaternionSlerp(qStart, qEnd, t);
    qInterpolated = DirectX::XMQuaternionNormalize(qInterpolated);

    DirectX::XMStoreFloat4(&qRot, qInterpolated);
    DirectX::XMStoreFloat4x4(&mRot, DirectX::XMMatrixRotationQuaternion(qInterpolated));

    UpdateMatrix();
}

void Transform::SetPosition(float x, float y, float z)
{
    position = { x, y, z };
    UpdateMatrix();
}

void Transform::LerpPosition(const DirectX::XMFLOAT3& target, float t)
{
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    position.x = position.x + (target.x - position.x) * t;
    position.y = position.y + (target.y - position.y) * t;
    position.z = position.z + (target.z - position.z) * t;

    UpdateMatrix();
}

void Transform::Translate(float x, float y, float z)
{
    position.x += x;
    position.y += y;
    position.z += z;
    UpdateMatrix();
}

void Transform::SetScale(float x, float y, float z)
{
    vSca = { x, y, z };
    UpdateMatrix();
}

void Transform::LookAt(const DirectX::XMFLOAT3& target)
{
    if (position.x == target.x && position.y == target.y && position.z == target.z)
    {
        std::cout << "[DEBUG] La position de la caméra est identique à la cible, LookAt ignoré.\n";
        return;
    }

    // Charger les vecteurs position et cible.
    DirectX::XMVECTOR posVec = DirectX::XMLoadFloat3(&position);
    DirectX::XMVECTOR targetVec = DirectX::XMLoadFloat3(&target);
    DirectX::XMVECTOR upVec = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    // Utilisation de XMMatrixLookAtLH pour calculer la matrice de vue.
    DirectX::XMMATRIX lookAtMatrix = DirectX::XMMatrixLookAtLH(posVec, targetVec, upVec);

    // Extraire les axes (Right, Up, Forward) de la matrice résultante.
    vRight = { lookAtMatrix.r[0].m128_f32[0], lookAtMatrix.r[0].m128_f32[1], lookAtMatrix.r[0].m128_f32[2] };
    vUp = { lookAtMatrix.r[1].m128_f32[0], lookAtMatrix.r[1].m128_f32[1], lookAtMatrix.r[1].m128_f32[2] };
    vDir = { -lookAtMatrix.r[2].m128_f32[0], -lookAtMatrix.r[2].m128_f32[1], -lookAtMatrix.r[2].m128_f32[2] };

    // Stocker la matrice dans mRot.
    DirectX::XMStoreFloat4x4(&mRot, lookAtMatrix);

    // Mettre à jour la matrice transform.
    UpdateMatrix();
}


DirectX::XMVECTOR Transform::GetPositionVector() const {
    return DirectX::XMLoadFloat3(&position);
}