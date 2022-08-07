#include "object/Rotator.h"


static void RegistObjectRotator(Schematyc::IEnvRegistrar& registrar)
{
    Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
    {
        Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(ceblanktc1::object::Rotator));
    }
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegistObjectRotator);
ceblanktc1::object::Rotator::Rotator(/* args */)
{
}
ceblanktc1::object::Rotator::~Rotator()
{
}
