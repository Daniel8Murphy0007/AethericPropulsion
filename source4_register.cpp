// Registration implementation for source4 Physics Terms
// Links together UQFF + MUGE + Navier-Stokes modules

#include "source4_wolfram.cpp"
#include "source4_wolfram_compressed.cpp"
#include "source4_wolfram_resonance.cpp"

// ============================================================================
// REGISTRATION FUNCTION IMPLEMENTATION
// ============================================================================

void registerSource4PhysicsTerms(PhysicsTermRegistry &registry)
{
    // ==== SOURCE4: 47 TOTAL CLASSES (24 base + 9 compressed + 13 resonance + 1 wormhole) ====

    // Source4 base UQFF physics (24 classes from source4_wolfram.cpp)

    // Core UQFF components (7)
    registry.registerPhysicsTerm("UniversalGravity1", std::make_unique<UniversalGravity1Term>(), "gravity_wolfram");
    registry.registerPhysicsTerm("UniversalGravity2", std::make_unique<UniversalGravity2Term>(), "gravity_wolfram");
    registry.registerPhysicsTerm("UniversalGravity3", std::make_unique<UniversalGravity3Term>(), "gravity_wolfram");
    registry.registerPhysicsTerm("UniversalGravity4", std::make_unique<UniversalGravity4Term>(), "gravity_wolfram");
    registry.registerPhysicsTerm("UniversalBuoyancy", std::make_unique<UniversalBuoyancyTerm>(), "gravity_wolfram");
    registry.registerPhysicsTerm("UniversalMagnetism", std::make_unique<UniversalMagnetismTerm>(), "gravity_wolfram");
    registry.registerPhysicsTerm("UniversalAether", std::make_unique<UniversalAetherTerm>(), "gravity_wolfram");

    // Unified field and MUGE systems (3)
    registry.registerPhysicsTerm("UnifiedField", std::make_unique<UnifiedFieldTerm>(), "unified_field");
    registry.registerPhysicsTerm("CompressedMUGE", std::make_unique<CompressedMUGETerm>(), "muge");
    registry.registerPhysicsTerm("ResonanceMUGE", std::make_unique<ResonanceMUGETerm>(), "muge");

    // Astrophysical systems (7)
    registry.registerPhysicsTerm("SGR1745Magnetar", std::make_unique<SGR1745MagnetarTerm>(), "astrophysics");
    registry.registerPhysicsTerm("SagittariusAStar", std::make_unique<SagittariusAStarTerm>(), "astrophysics");
    registry.registerPhysicsTerm("TapestryStarbirth", std::make_unique<TapestryStarbirthTerm>(), "astrophysics");
    registry.registerPhysicsTerm("Westerlund2Cluster", std::make_unique<Westerlund2ClusterTerm>(), "astrophysics");
    registry.registerPhysicsTerm("PillarsCreation", std::make_unique<PillarsCreationTerm>(), "astrophysics");
    registry.registerPhysicsTerm("RingsRelativity", std::make_unique<RingsRelativityTerm>(), "astrophysics");
    registry.registerPhysicsTerm("StudentGuideUniverse", std::make_unique<StudentGuideUniverseTerm>(), "astrophysics");

    // Helper functions (7)
    registry.registerPhysicsTerm("MuS", std::make_unique<MuSTerm>(), "helper");
    registry.registerPhysicsTerm("GradMsR", std::make_unique<GradMsRTerm>(), "helper");
    registry.registerPhysicsTerm("Bj", std::make_unique<BjTerm>(), "helper");
    registry.registerPhysicsTerm("OmegaST", std::make_unique<OmegaSTTerm>(), "helper");
    registry.registerPhysicsTerm("MuJ", std::make_unique<MuJTerm>(), "helper");
    registry.registerPhysicsTerm("ReactorEfficiency", std::make_unique<ReactorEfficiencyTerm>(), "helper");
    registry.registerPhysicsTerm("NavierStokesQuasarJet", std::make_unique<NavierStokesQuasarJetTerm>(), "helper");

    // MUGE compressed terms (9 classes from source4_wolfram_compressed.cpp)
    registry.registerPhysicsTerm("MUGECompressedBase", std::make_unique<MUGECompressedBaseTerm>(), "muge_compressed");
    registry.registerPhysicsTerm("MUGEExpansion", std::make_unique<MUGEExpansionTerm>(), "muge_compressed");
    registry.registerPhysicsTerm("MUGESuperAdjustment", std::make_unique<MUGESuperAdjustmentTerm>(), "muge_compressed");
    registry.registerPhysicsTerm("MUGEEnvelope", std::make_unique<MUGEEnvelopeTerm>(), "muge_compressed");
    registry.registerPhysicsTerm("MUGEUgSum", std::make_unique<MUGEUgSumTerm>(), "muge_compressed");
    registry.registerPhysicsTerm("MUGECosmological", std::make_unique<MUGECosmologicalTerm>(), "muge_compressed");
    registry.registerPhysicsTerm("MUGEQuantum", std::make_unique<MUGEQuantumTerm>(), "muge_compressed");
    registry.registerPhysicsTerm("MUGEFluid", std::make_unique<MUGEFluidTerm>(), "muge_compressed");
    registry.registerPhysicsTerm("MUGEPerturbation", std::make_unique<MUGEPerturbationTerm>(), "muge_compressed");

    // MUGE resonance terms (13 classes from source4_wolfram_resonance.cpp)
    registry.registerPhysicsTerm("MUGEResonanceADPM", std::make_unique<MUGEResonanceADPMTerm>(), "muge_resonance");
    registry.registerPhysicsTerm("MUGEResonanceATHz", std::make_unique<MUGEResonanceATHzTerm>(), "muge_resonance");
    registry.registerPhysicsTerm("MUGEResonanceAvacDiff", std::make_unique<MUGEResonanceAvacDiffTerm>(), "muge_resonance");
    registry.registerPhysicsTerm("MUGEResonanceASuperFreq", std::make_unique<MUGEResonanceASuperFreqTerm>(), "muge_resonance");
    registry.registerPhysicsTerm("MUGEResonanceAAetherRes", std::make_unique<MUGEResonanceAAetherResTerm>(), "muge_resonance");
    registry.registerPhysicsTerm("MUGEResonanceUg4i", std::make_unique<MUGEResonanceUg4iTerm>(), "muge_resonance");
    registry.registerPhysicsTerm("MUGEResonanceAQuantumFreq", std::make_unique<MUGEResonanceAQuantumFreqTerm>(), "muge_resonance");
    registry.registerPhysicsTerm("MUGEResonanceAAetherFreq", std::make_unique<MUGEResonanceAAetherFreqTerm>(), "muge_resonance");
    registry.registerPhysicsTerm("MUGEResonanceAFluidFreq", std::make_unique<MUGEResonanceAFluidFreqTerm>(), "muge_resonance");
    registry.registerPhysicsTerm("MUGEResonanceOsc", std::make_unique<MUGEResonanceOscTerm>(), "muge_resonance");
    registry.registerPhysicsTerm("MUGEResonanceAExpFreq", std::make_unique<MUGEResonanceAExpFreqTerm>(), "muge_resonance");
    registry.registerPhysicsTerm("MUGEResonanceFTRZ", std::make_unique<MUGEResonanceFTRZTerm>(), "muge_resonance");
    registry.registerPhysicsTerm("MUGEResonanceWormhole", std::make_unique<MUGEResonanceWormholeTerm>(), "muge_resonance");
}
