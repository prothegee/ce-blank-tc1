#pragma once
#include "ceblanktc1.h"


namespace ceblanktc1
{
namespace object
{


class Rotator : public IEntityComponent
{
public:
    struct DVRotator
    {
        static constexpr bool m_isRotating = false;
        static constexpr float m_rotationX = 0.0f;
        static constexpr float m_rotationY = 0.0f;
        static constexpr float m_rotationZ = 0.0f;
    };

    bool m_isRotating = DVRotator::m_isRotating;
    Schematyc::Range<-100, 100, -100, 100, float> m_rotationX = DVRotator::m_rotationX;
    Schematyc::Range<-100, 100, -100, 100, float> m_rotationY = DVRotator::m_rotationY;
    Schematyc::Range<-100, 100, -100, 100, float> m_rotationZ = DVRotator::m_rotationZ;
    

public:
    Rotator(/* args */);
    virtual ~Rotator();

    virtual void Initialize() override {}

    static void ReflectType(Schematyc::CTypeDesc<Rotator>& desc)
    {
        desc.SetGUID("{ca5633c2-6aca-4c93-9598-c3fd83efc110}"_cry_guid);
        desc.SetLabel("object rotator");
        desc.SetEditorCategory("_objects");
        desc.SetDescription("object rotator entity component");
        #pragma region data member
        desc.AddMember(
            &Rotator::m_isRotating,
            'oirt',
            "object-is-rotating",
            "is rotating",
            "is rotating value",
            DVRotator::m_isRotating);
        desc.AddMember(
            &Rotator::m_rotationX,
            'ortx',
            "object-rotation-x",
            "rotation x",
            "rotation x value",
            DVRotator::m_rotationX);
        desc.AddMember(
            &Rotator::m_rotationY,
            'orty',
            "object-rotation-y",
            "rotation y",
            "rotation y value",
            DVRotator::m_rotationY);
        desc.AddMember(
            &Rotator::m_rotationZ,
            'ortz',
            "object-rotation-z",
            "rotation z",
            "rotation z value",
            DVRotator::m_rotationZ);
        #pragma endregion
    }

    virtual Cry::Entity::EventFlags GetEventMask() const override
    {
        return
            Cry::Entity::EEvent::GameplayStarted |
            Cry::Entity::EEvent::EditorPropertyChanged |
            Cry::Entity::EEvent::Reset |
            Cry::Entity::EEvent::Update
            ;
    }

    virtual void ProcessEvent(const SEntityEvent& e) override
    {
        switch (e.event)
        {
            case Cry::Entity::EEvent::GameplayStarted:
            {

            }
            break;

            case Cry::Entity::EEvent::EditorPropertyChanged:
            {
                Initialize();
            }
            break;

            case Cry::Entity::EEvent::Reset:
            {
                
            }
            break;

            case Cry::Entity::EEvent::Update:
            {
                if (m_isRotating)
                {
                    const float dt = e.fParam[0];

                    Ang3 ar = ZERO;

                    ar.x += m_rotationX * dt;
                    ar.y += m_rotationY * dt;
                    ar.z += m_rotationZ * dt;

                    auto r = GetEntity()->GetRotation() * Quat::CreateRotationXYZ(ar);

                    m_pEntity->SetRotation(r);
                }
            }
            break;
        }
    }
}; // class Rotator


} // namespace object
} // namespace ceblanktc1
