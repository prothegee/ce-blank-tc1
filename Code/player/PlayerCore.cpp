#include "PlayerCore.h"


static void RegistPlayerCore(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(PlayerCore));
    }
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegistPlayerCore);
PlayerCore::PlayerCore(/* args */)
{
}
PlayerCore::~PlayerCore()
{
}




void PlayerCore::Initialize()
{
    m_pInput = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CInputComponent>();
    m_pCC = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCharacterControllerComponent>();
    m_pCamera = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCameraComponent>();
    m_pAudio = m_pEntity->GetOrCreateComponent<Cry::Audio::DefaultComponents::CListenerComponent>();
}


Cry::Entity::EventFlags PlayerCore::GetEventMask() const
{
    return
        Cry::Entity::EEvent::GameplayStarted |
        Cry::Entity::EEvent::Reset |
        Cry::Entity::EEvent::Update
        ;
}


void PlayerCore::ProcessEvent(const SEntityEvent& e)
{
    switch (e.event)
    {
        case Cry::Entity::EEvent::GameplayStarted:
        {
            InitializePlayerInput();
        }
        break;

        case Cry::Entity::EEvent::Reset:
        {
            InitializePlayerInput();

            #ifndef NDEBUG
            CryLog("# m_spawnOnCamera: %s", m_spawnOnCamera ? "true" : "false");
            #else
            #endif
        }
        break;

        case Cry::Entity::EEvent::Update:
        {
            const float dt = e.fParam[0];

            GroundMovementHandler(dt);
            OrientHandler(dt);
        }
        break;
    }
}




void PlayerCore::InitializePlayerInput()
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
    #pragma endregion
}


void PlayerCore::ResetPlayerInput()
{
    m_mouseDeltaRotation = ZERO;
    m_entityDeltaRotation = ZERO;

    m_lookOrientation = IDENTITY;
    m_entityLookOrientation = IDENTITY;
}


void PlayerCore::HandleInputFlagChange(CEnumFlags<EInputFlag> flags, CEnumFlags<EActionActivationMode> activationMode, EInputFlagType type)
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




void PlayerCore::GroundMovementHandler(float dt)
{
    if (!m_pCC->IsOnGround()) { return; }

    Vec3 velocity = ZERO;

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


    m_pCC->AddVelocity(GetEntity()->GetWorldRotation() * velocity);
}


void PlayerCore::OrientHandler(float dt)
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
