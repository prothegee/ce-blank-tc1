#include "PlayerCore.h"


static void RegistPlayerCore(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(ceblanktc1::player::PlayerCore));
    }
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegistPlayerCore);
ceblanktc1::player::PlayerCore::PlayerCore(/* args */)
{
}
ceblanktc1::player::PlayerCore::~PlayerCore()
{
}




void ceblanktc1::player::PlayerCore::Initialize()
{
    m_pInput = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CInputComponent>();
    m_pCC = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCharacterControllerComponent>();
    m_pCamera = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCameraComponent>();
    m_pAudio = m_pEntity->GetOrCreateComponent<Cry::Audio::DefaultComponents::CListenerComponent>();
}


Cry::Entity::EventFlags ceblanktc1::player::PlayerCore::GetEventMask() const
{
    return
        Cry::Entity::EEvent::GameplayStarted |
        Cry::Entity::EEvent::Reset |
        Cry::Entity::EEvent::Update
        ;
}


void ceblanktc1::player::PlayerCore::ProcessEvent(const SEntityEvent& e)
{
    switch (e.event)
    {
        case Cry::Entity::EEvent::GameplayStarted:
        {
            InitializePlayerInput();
            PlayerSpawnConditions();

            #ifndef NDEBUG
            CryLog("# version: v%s", cfg.GetProjectVersion());
            CryLog("# current map: %s", lvlman.GetCurrentLevel().c_str());
            CryLog("# current time: %s", dnt.current_time_utc.c_str());
            #else
            #endif
        }
        break;

        case Cry::Entity::EEvent::Reset:
        {
            InitializePlayerInput();

            #ifndef NDEBUG
            #else
            #endif
        }
        break;

        case Cry::Entity::EEvent::Update:
        {
            const float dt = e.fParam[0];

            GroundMovementHandler(dt);
            OrientHandler(dt);

            PlayerStateConditions();
            PlayerDataPolicies(dt);
        }
        break;
    }
}




void ceblanktc1::player::PlayerCore::InitializePlayerInput()
{
    #pragma region input register and binding
    // move forward
    m_pInput->RegisterAction(
        "player",
        "move-forward",
        [this](int activationMode, float value)
        {
            HandleInputFlagChange(
                EInputFlag::MoveForward,
                (EActionActivationMode)activationMode);
        });
        m_pInput->BindAction(
            "player",
            "move-forward",
            EActionInputDevice::eAID_KeyboardMouse,
            EKeyId::eKI_W);

    // move backward
    m_pInput->RegisterAction(
        "player",
        "move-backward",
        [this](int activationMode, float value)
        {
            HandleInputFlagChange(
                EInputFlag::MoveBackward,
                (EActionActivationMode)activationMode);
        });
        m_pInput->BindAction(
            "player",
            "move-backward",
            EActionInputDevice::eAID_KeyboardMouse,
            EKeyId::eKI_S);

    // move left
    m_pInput->RegisterAction(
        "player",
        "move-left",
        [this](int activationMode, float value)
        {
            HandleInputFlagChange(
                EInputFlag::MoveLeft,
                (EActionActivationMode)activationMode);
        });
        m_pInput->BindAction(
            "player",
            "move-left",
            EActionInputDevice::eAID_KeyboardMouse,
            EKeyId::eKI_A);

    // move right
    m_pInput->RegisterAction(
        "player",
        "move-right",
        [this](int activationMode, float value)
        {
            HandleInputFlagChange(
                EInputFlag::MoveRight,
                (EActionActivationMode)activationMode);
        });
        m_pInput->BindAction(
            "player",
            "move-right",
            EActionInputDevice::eAID_KeyboardMouse,
            EKeyId::eKI_D);

    // mouse rorate yaw
    m_pInput->RegisterAction(
        "player",
        "mouse-rotate-yaw",
        [this](int activationMode, float value)
        {
            m_mouseDeltaRotation.x -= value;
            m_entityDeltaRotation.x -= value;

            // check rotation to trigger root/character model to rotate
        });
        m_pInput->BindAction(
            "player",
            "mouse-rotate-yaw",
            EActionInputDevice::eAID_KeyboardMouse,
            EKeyId::eKI_MouseX);

    // mouse rorate pitch
	m_pInput->RegisterAction(
        "player",
        "mouse-rotate-pitch",
        [this](int activationMode, float value)
        {
            m_mouseDeltaRotation.y -= value;
        });
        m_pInput->BindAction(
            "player",
            "mouse-rotate-pitch",
            EActionInputDevice::eAID_KeyboardMouse,
            EKeyId::eKI_MouseY);

    // sprint
    m_pInput->RegisterAction(
        "player",
        "sprint",
        [this](int activationMode, float value)
        {
            HandleInputFlagChange(
                EInputFlag::DoSprint,
                (EActionActivationMode)activationMode);
        });
        m_pInput->BindAction(
            "player",
            "sprint",
            EActionInputDevice::eAID_KeyboardMouse,
            EKeyId::eKI_LShift);

    // jump
    m_pInput->RegisterAction(
        "player",
        "jump",
        [this](int activationMode, float value)
        {
            HandleInputFlagChange(
                EInputFlag::DoJump,
                (EActionActivationMode)activationMode);
        });
        m_pInput->BindAction(
            "player",
            "jump",
            EActionInputDevice::eAID_KeyboardMouse,
            EKeyId::eKI_Space);

    // shot
    m_pInput->RegisterAction(
        "player",
        "shoot",
        [this](int activationMode, float value)
        {
            if (activationMode == eAAM_OnPress)
            {
                #ifndef NDEBUG
                CryLog("# shoot");
                #else
                #endif
            }
        });
        m_pInput->BindAction(
            "player",
            "shoot",
            EActionInputDevice::eAID_KeyboardMouse,
            EKeyId::eKI_Mouse1);

    // aim
    m_pInput->RegisterAction(
        "player",
        "aim",
        [this](int activationMode, float value)
        {
            if (activationMode == eAAM_OnPress)
            {
                if (m_aimStance)
                {
                    m_aimStance = false;
                }
                else
                {
                    m_aimStance = true;
                }

                #ifndef NDEBUG
                CryLog("# m_aimStance %s", m_aimStance ? "true" : "false");
                #else
                #endif
            }
        });
        m_pInput->BindAction(
            "player",
            "aim",
            EActionInputDevice::eAID_KeyboardMouse,
            EKeyId::eKI_Mouse2);
    #pragma endregion
}


void ceblanktc1::player::PlayerCore::ResetPlayerInput()
{
    m_mouseDeltaRotation = ZERO;
    m_entityDeltaRotation = ZERO;

    m_lookOrientation = IDENTITY;
    m_entityLookOrientation = IDENTITY;
}


void ceblanktc1::player::PlayerCore::HandleInputFlagChange(CEnumFlags<EInputFlag> flags, CEnumFlags<EActionActivationMode> activationMode, EInputFlagType type)
{
    switch (type)
    {
        case EInputFlagType::Hold:
        {
            if (activationMode == EActionActivationMode::eAAM_OnRelease)
            {
                m_inputFlags &= ~flags;
            }
            else
            {
                m_inputFlags |= flags;
            }
        }
        break;

        case EInputFlagType::Toggle:
        {
            if (activationMode == EActionActivationMode::eAAM_OnRelease)
            {
                m_inputFlags ^= flags;
            }
        }
        break;
    }
}




void ceblanktc1::player::PlayerCore::GroundMovementHandler(float dt)
{
    if (!m_pCC->IsOnGround()) { return; }

    Vec3 velocity = ZERO;

    #pragma region base movement
    if (m_inputFlags & EInputFlag::MoveLeft)
    {
        velocity.x -= m_movementSpeed * dt;
    }
    if (m_inputFlags & EInputFlag::MoveRight)
    {
        velocity.x += m_movementSpeed * dt;
    }
    if (m_inputFlags & EInputFlag::MoveForward)
    {
        velocity.y += m_movementSpeed * dt;
    }
    if (m_inputFlags & EInputFlag::MoveBackward)
    {
        velocity.y -= m_movementSpeed * dt;
    }
    #pragma endregion


    #pragma region jump
    float tmpjumpcharge = 0.0f; // temporal jump charge
    /**
     * this jump logic had consequence to be a bit delay since using charge jump
     */
    if (m_inputFlags & EInputFlag::DoJump)
    {
        m_jumpDurationOnHold += 1.f * dt;

        (m_jumpDurationOnHold >= m_minJumpCharge)
            ?   
                tmpjumpcharge = m_jumpDurationOnHold,
                m_canJumpNow = true,
                m_stamina -= m_staminaReductionRate * dt
            :   
                tmpjumpcharge = 1.f,
                m_canJumpNow = true,
                m_stamina -= m_staminaReductionRate * dt;

        (tmpjumpcharge >= m_maxJumpCharge)
            ? m_jumpCharge = m_maxJumpCharge
            : m_jumpCharge = tmpjumpcharge;
    }
    else
    {
        m_jumpDurationOnHold = 0.f;
    }

    if (m_canJumpNow && m_jumpDurationOnHold == 0.0f && m_stamina >= m_staminaMinValue)
    {
        if (m_pCC->IsOnGround())
        {
            m_pCC->AddVelocity(Vec3(0, 0, ((m_jumpForce * m_jumpCharge) * m_jumpChargeMultiplier)));
        }

        // reset
        m_canJumpNow = false;
        m_jumpDurationOnHold = 0.0f;
        m_jumpCharge = 0.0f;
        m_jumpChargeMultiplier = DVPlayerCore::m_jumpChargeMultiplier;
        tmpjumpcharge = 0.0f;
    }
    #pragma endregion


    #pragma region sprint
    if (m_inputFlags & EInputFlag::DoSprint)
    {
        m_aimStance = false; // break the aim stance
        

        if (m_stamina > m_staminaMinValue)
            m_pCC->AddVelocity(GetEntity()->GetWorldRotation() * (velocity * DVPlayerCore::m_sprintMultiplier));

            // reduce stamina when sprint
            if (m_pCC->GetVelocity() != Vec3(0, 0, 0))
            {
                m_stamina -= m_staminaReductionRate * dt;
            }
    }
    else
    {
        m_pCC->AddVelocity(GetEntity()->GetWorldRotation() * velocity);
    }
    #pragma endregion
}


void ceblanktc1::player::PlayerCore::OrientHandler(float dt)
{
    #pragma region camera v1 section
    // camera entity world tranformation
    Matrix34 ctf = m_pCamera->GetTransformMatrix();
    Ang3 rypr = CCamera::CreateAnglesYPR(Matrix33(m_lookOrientation));

    float rotationLimitsMinPitch = m_rotationLimitsMinPitch;
    float rotationLimitsMaxPitch = m_rotationLimitsMaxPitch;

    rypr.x = 0; // x axis already handle #pragma region root or parent section
    rypr.y = CLAMP(rypr.y + m_mouseDeltaRotation.y * (m_rotationSpeed * m_sensitivity), rotationLimitsMinPitch, rotationLimitsMaxPitch);
    rypr.z = 0;

    m_lookOrientation = Quat(CCamera::CreateOrientationYPR(rypr));
    m_mouseDeltaRotation = ZERO;

    ctf.SetRotation33(CCamera::CreateOrientationYPR(rypr));

    m_pCamera->SetTransformMatrix(ctf);
    m_pAudio->SetOffset(ctf.GetTranslation());
    #pragma endregion

    #pragma region root or parent v1 section
    // core/root entity world tranformation
    Matrix34 etf = m_pEntity->GetWorldTM();
    Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(m_entityLookOrientation));

    ypr.x += m_entityDeltaRotation.x * (m_rotationSpeed * m_sensitivity);
    ypr.y = 0;
    ypr.z = 0;

    m_entityLookOrientation = Quat(CCamera::CreateOrientationYPR(ypr));
    m_entityDeltaRotation = ZERO;

    etf.SetRotation33(CCamera::CreateOrientationYPR(ypr));

    m_pEntity->SetWorldTM(etf);
    #pragma endregion
}




void ceblanktc1::player::PlayerCore::PlayerStateConditions()
{
    // health
    (m_health <= m_healthMinValue)
        ? m_isDied = true
        : m_isDied = false;

    // stamina
    (m_stamina <= m_staminaMinValue)
        ? m_isOutOfStamina = true
        : m_isOutOfStamina = false;
}


void ceblanktc1::player::PlayerCore::PlayerSpawnConditions()
{
    Matrix34 spawnPointTranformation;

    if (m_spawnOnCamera)
    {
        spawnPointTranformation = gEnv->pSystem->GetViewCamera().GetMatrix();
    }
    else
    {
        spawnPointTranformation = m_pEntity->GetWorldTM();
    }

    m_pEntity->SetWorldTM(spawnPointTranformation);
}


void ceblanktc1::player::PlayerCore::PlayerDataPolicies(float dt)
{
    // health
    if (m_health <= m_healthToRegenerate)
    {
        m_health += m_healthRegenerationRate * dt;
    }

    // stamina
    if (m_stamina < m_staminaMaxValue)
    {
        m_stamina += m_staminaRegenerationRate * dt;
    }

    // aim stance
    (m_aimStance)
        ? m_movementSpeed = DVPlayerCore::m_movementSpeed/2
        : m_movementSpeed = DVPlayerCore::m_movementSpeed;
}
