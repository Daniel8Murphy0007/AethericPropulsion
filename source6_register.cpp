// Registration implementation for source6 Physics Terms
// Links together main infrastructure with graphics and physics modules

#include "source6_wolfram_graphics.cpp"
#include "source6_wolfram_physics.cpp"

// ============================================================================
// REGISTRATION FUNCTION IMPLEMENTATION
// ============================================================================

void registerSource6PhysicsTerms(PhysicsTermRegistry &registry)
{
    // ==== HYBRID SOURCE6: 29 TOTAL CLASSES (14 graphics + 15 physics) ====

    // Source6-specific graphics infrastructure (14 classes)
    registry.registerPhysicsTerm("OpenGLRender", std::make_unique<OpenGLRenderTerm>(), "graphics");
    registry.registerPhysicsTerm("VulkanRender", std::make_unique<VulkanRenderTerm>(), "graphics");
    registry.registerPhysicsTerm("MeshLoaderOBJ", std::make_unique<MeshLoaderOBJTerm>(), "graphics");
    registry.registerPhysicsTerm("ProceduralLandscape", std::make_unique<ProceduralLandscapeTerm>(), "graphics");
    registry.registerPhysicsTerm("MeshExtrude", std::make_unique<MeshExtrudeTerm>(), "graphics");
    registry.registerPhysicsTerm("MeshBoolean", std::make_unique<MeshBooleanTerm>(), "graphics");
    registry.registerPhysicsTerm("TextureLoader", std::make_unique<TextureLoaderTerm>(), "graphics");
    registry.registerPhysicsTerm("ShaderCompile", std::make_unique<ShaderCompileTerm>(), "graphics");
    registry.registerPhysicsTerm("CameraViewMatrix", std::make_unique<CameraViewMatrixTerm>(), "graphics");
    registry.registerPhysicsTerm("BoneAnimation", std::make_unique<BoneAnimationTerm>(), "graphics");
    registry.registerPhysicsTerm("LaTeXRender", std::make_unique<LaTeXRenderTerm>(), "graphics");
    registry.registerPhysicsTerm("MultiViewport", std::make_unique<MultiViewportTerm>(), "graphics");
    registry.registerPhysicsTerm("SimulationEntityUpdate", std::make_unique<SimulationEntityUpdateTerm>(), "graphics");
    registry.registerPhysicsTerm("ToolPathExecution", std::make_unique<ToolPathExecutionTerm>(), "graphics");

    // Source6-specific UQFF physics (15 classes extracted from source6.cpp)
    // Helpers (7)
    registry.registerPhysicsTerm("StepFunctionSource6", std::make_unique<StepFunctionSource6Term>(), "helper");
    registry.registerPhysicsTerm("ReactorEnergySource6", std::make_unique<ReactorEnergySource6Term>(), "helper");
    registry.registerPhysicsTerm("MagneticMomentTimeSource6", std::make_unique<MagneticMomentTimeSource6Term>(), "helper");
    registry.registerPhysicsTerm("GradientMassRadiusSource6", std::make_unique<GradientMassRadiusSource6Term>(), "helper");
    registry.registerPhysicsTerm("MagneticJetFieldSource6", std::make_unique<MagneticJetFieldSource6Term>(), "helper");
    registry.registerPhysicsTerm("OmegaSpinModulationSource6", std::make_unique<OmegaSpinModulationSource6Term>(), "helper");
    registry.registerPhysicsTerm("MagneticJetMomentSource6", std::make_unique<MagneticJetMomentSource6Term>(), "helper");

    // Core UQFF (8)
    registry.registerPhysicsTerm("UniversalGravity1Source6", std::make_unique<UniversalGravity1Source6Term>(), "gravity_wolfram");
    registry.registerPhysicsTerm("UniversalGravity2Source6", std::make_unique<UniversalGravity2Source6Term>(), "gravity_wolfram");
    registry.registerPhysicsTerm("UniversalGravity3Source6", std::make_unique<UniversalGravity3Source6Term>(), "gravity_wolfram");
    registry.registerPhysicsTerm("UniversalGravity4Source6", std::make_unique<UniversalGravity4Source6Term>(), "gravity_wolfram");
    registry.registerPhysicsTerm("UniversalBuoyancySource6", std::make_unique<UniversalBuoyancySource6Term>(), "gravity_wolfram");
    registry.registerPhysicsTerm("UniversalMagnetismSource6", std::make_unique<UniversalMagnetismSource6Term>(), "gravity_wolfram");
    registry.registerPhysicsTerm("SpacetimeMetricSource6", std::make_unique<SpacetimeMetricSource6Term>(), "gravity_wolfram");
    registry.registerPhysicsTerm("FullUnifiedFieldSource6", std::make_unique<FullUnifiedFieldSource6Term>(), "unified_field");
}
