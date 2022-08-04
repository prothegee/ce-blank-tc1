#pragma once
#include "project.h"


class PlayerCore : public IEntityComponent
{
    struct DVPlayerCore
    {
        static constexpr float m_health = 100.0f;
        static constexpr float m_stamina = 100.0f;
        static constexpr float m_movementSpeed = 50.0f;
        
        static constexpr float m_sensitivity = 1.0f;
        static constexpr float m_rotationLimitsMinPitch = -0.85f;
        static constexpr float m_rotationLimitsMaxPitch = 1.6f;
    };

    enum class EInputFlagType
    {
        Hold = 0,
        Toggle
    };

    enum class EInputFlag : uint8
    {
        MoveLeft     = 1 << 0,
        MoveRight    = 1 << 1,
        MoveForward  = 1 << 2,
        MoveBackward = 1 << 3,
    };


private:
    bool m_spawnOnCamera = [](){ return (gEnv->IsEditor()) ? true : false; }();


public:
    PlayerCore(/* args */);
    virtual ~PlayerCore();

    static void ReflectType(Schematyc::CTypeDesc<PlayerCore>& desc)
    {
        desc.SetGUID("{ba9a9e77-0969-4b5e-95df-c7dcca5ffab9}"_cry_guid);
        desc.SetLabel("player core component");
        desc.SetEditorCategory("_players");
        desc.SetDescription("player core component");
        desc.AddMember(
            &PlayerCore::m_health,
            'hlth',
            "player-health",
            "health",
            "health value",
            DVPlayerCore::m_health);
        desc.AddMember(
            &PlayerCore::m_stamina,
            'stmn',
            "player-stamina",
            "stamina",
            "stamina value",
            DVPlayerCore::m_stamina);
        desc.AddMember(
            &PlayerCore::m_movementSpeed,
            'mvmn',
            "player-movement-speed",
            "movement speed",
            "movement speed value",
            DVPlayerCore::m_movementSpeed);
        desc.AddMember(
            &PlayerCore::m_sensitivity,
            'sntv',
            "player-controller-sensitivity",
            "sensitivity",
            "sensitivity multiplier value",
            DVPlayerCore::m_sensitivity);
        desc.AddMember(
            &PlayerCore::m_rotationLimitsMinPitch,
            'rmin',
            "player-controller-rotation-min-pitch",
            "rotation min pitch",
            "rotation min pitch value",
            -0.85f);
        desc.AddMember(
            &PlayerCore::m_rotationLimitsMaxPitch,
            'rmax',
            "player-controller-rotation-max-pitch",
            "rotation max pitch",
            "rotation max pitch value",
            1.6f);
        
    }

    virtual void Initialize() override;
    virtual Cry::Entity::EventFlags GetEventMask() const override;
    virtual void ProcessEvent(const SEntityEvent& e) override;


protected:
    Schematyc::Range<0, 100, 0, 100, float> m_health = DVPlayerCore::m_health;
    Schematyc::Range<0, 100, 0, 100, float> m_stamina = DVPlayerCore::m_stamina;
    Schematyc::Range<0, 100, 0, 100, float> m_movementSpeed = DVPlayerCore::m_movementSpeed;

    Schematyc::Range<0, 100, 0, 100, float> m_sensitivity = DVPlayerCore::m_sensitivity;
    Schematyc::Range<-1, 1, -1, 1, float> m_rotationLimitsMinPitch = DVPlayerCore::m_rotationLimitsMinPitch;
    Schematyc::Range<-2, 2, -2, 2, float> m_rotationLimitsMaxPitch = DVPlayerCore::m_rotationLimitsMaxPitch;

    const float m_rotationSpeed = 0.002f;

    Vec2 m_mouseDeltaRotation = ZERO;
    Quat m_lookOrientation = IDENTITY;
    Vec2 m_entityDeltaRotation = ZERO;
    Quat m_entityLookOrientation = IDENTITY;

    CEnumFlags<EInputFlag> m_inputFlags;

    Cry::DefaultComponents::CInputComponent* m_pInput = nullptr;
    Cry::DefaultComponents::CCharacterControllerComponent* m_pCC = nullptr;
    Cry::DefaultComponents::CCameraComponent* m_pCamera = nullptr;
    Cry::Audio::DefaultComponents::CListenerComponent* m_pAudio = nullptr;

protected:
    void InitializePlayerInput();
    void ResetPlayerInput();
    void HandleInputFlagChange(CEnumFlags<EInputFlag> flags, CEnumFlags<EActionActivationMode> activationMode, EInputFlagType type = EInputFlagType::Hold);

protected:
    void GroundMovementHandler(float dt);
    void OrientHandler(float dt);
};
