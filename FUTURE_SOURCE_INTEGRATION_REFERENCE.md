# Future Source File Integration Reference Guide

**AethericPropulsion UQFF Framework**  
**Version**: 1.0  
**Created**: December 10, 2025  
**Purpose**: Standardized procedure for integrating SOURCE7+ modular frameworks

---

## Quick Start Checklist

When adding a new SourceN framework to the project:

- [ ] **Phase 1**: Create modular files (4-6 files)
- [ ] **Phase 2**: Implement PhysicsTerm classes
- [ ] **Phase 3**: Create registration function
- [ ] **Phase 4**: Update MAIN_1_CoAnQi.cpp (2 edits)
- [ ] **Phase 5**: Update documentation (3 files)
- [ ] **Phase 6**: Test compilation and registration
- [ ] **Phase 7**: Commit with standardized message

---

## Phase 1: Modular File Structure

### Standard File Naming Convention

```
sourceN.cpp                        - Base implementation (CelestialBody struct, compute functions)
sourceN_wolfram.cpp                - Base infrastructure (PhysicsTerm base, registry)
sourceN_wolfram_[category].cpp     - Category-specific classes (graphics/physics/compressed/resonance)
sourceN_register.cpp               - Registration function implementation
sourceN_simulation_harness.cpp     - Optional: Interactive testing framework
sourceN_[category]_SPEC.md         - Optional: Complete specification document
```

### Category Examples

- `sourceN_wolfram_graphics.cpp` - Graphics/visualization classes
- `sourceN_wolfram_physics.cpp` - Core UQFF physics classes
- `sourceN_wolfram_compressed.cpp` - MUGE compressed components
- `sourceN_wolfram_resonance.cpp` - MUGE resonance components
- `sourceN_wolfram_helpers.cpp` - Utility/helper functions
- `sourceN_wolfram_systems.cpp` - Astrophysical system definitions

---

## Phase 2: PhysicsTerm Class Implementation

### Standard PhysicsTerm Class Template

```cpp
class MyNewPhysicsTermSourceN : public PhysicsTerm
{
private:
    // Physical parameters (with SI units in comments)
    double param1;  // Parameter description (units)
    double param2;  // Parameter description (units)
    // ... more parameters

public:
    // Constructor with default values
    MyNewPhysicsTermSourceN(double p1 = default_value, double p2 = default_value)
        : param1(p1), param2(p2)
    {
        // Metadata for tracking and organization
        setMetadata("source", "sourceN.cpp");
        setMetadata("type", "physics_category");
        setMetadata("physics", "physics_subcategory");
    }

    // Compute function - main physics calculation
    double compute(double t, const std::map<std::string, double> &params) const override
    {
        // Extract runtime parameters (with defaults)
        double runtime_param = params.count("param_name") ? params.at("param_name") : default_value;

        // Physics calculation
        double result = /* your physics equation here */;

        return result;
    }

    // Descriptive name (used in registry lookups)
    std::string getName() const override { return "MyNewPhysicsTerm_SourceN"; }

    // Human-readable description (displayed in logs/UI)
    std::string getDescription() const override
    {
        return "Brief description of what this term calculates";
    }

    // Optional: Parameter validation
    bool validate(const std::map<std::string, double> &params) const override
    {
        // Check for required parameters or physical constraints
        if (params.count("required_param") == 0) return false;
        if (params.at("some_param") < 0) return false;  // Physical constraint
        return true;
    }
};
```

### Metadata Categories

**"source"**: Which source file originated the class

- Values: `"sourceN.cpp"`, `"embedded"`, `"wolfram_generated"`

**"type"**: Primary physics category

- Values: `"unified_field"`, `"gravity_wolfram"`, `"compressed_muge"`, `"resonance_muge"`, `"helper"`, `"graphics"`, `"system"`, `"utility"`

**"physics"**: Specific physics subcategory

- Examples: `"magnetic_dipole_mass_gradient"`, `"ua_charge_scm_interaction"`, `"navier_stokes_fluid"`, `"vacuum_energy_differential"`

---

## Phase 3: Registration Function

### Standard Registration Function Template

```cpp
// File: sourceN_register.cpp

#include "sourceN_wolfram.cpp"           // Base infrastructure
#include "sourceN_wolfram_graphics.cpp"  // Graphics classes (if exists)
#include "sourceN_wolfram_physics.cpp"   // Physics classes (if exists)
// ... include other category files

// ============================================================================
// REGISTRATION FUNCTION IMPLEMENTATION
// ============================================================================

void registerSourceNPhysicsTerms(PhysicsTermRegistry &registry)
{
    // ==== SOURCEN: X TOTAL CLASSES (breakdown by category) ====

    // Category 1: Graphics Infrastructure (if applicable)
    registry.registerPhysicsTerm("ClassName1", std::make_unique<ClassName1Term>(), "graphics");
    registry.registerPhysicsTerm("ClassName2", std::make_unique<ClassName2Term>(), "graphics");
    // ... more graphics classes

    // Category 2: Helper Functions
    registry.registerPhysicsTerm("HelperName1", std::make_unique<HelperName1TermSourceN>(), "helper");
    registry.registerPhysicsTerm("HelperName2", std::make_unique<HelperName2TermSourceN>(), "helper");
    // ... more helper classes

    // Category 3: Core UQFF Physics
    registry.registerPhysicsTerm("CorePhysics1", std::make_unique<CorePhysics1TermSourceN>(), "gravity_wolfram");
    registry.registerPhysicsTerm("CorePhysics2", std::make_unique<CorePhysics2TermSourceN>(), "gravity_wolfram");
    // ... more core physics classes

    // Category 4: Compressed MUGE (if applicable)
    registry.registerPhysicsTerm("Compressed1", std::make_unique<Compressed1TermSourceN>(), "compressed_muge");
    // ... more compressed classes

    // Category 5: Resonance MUGE (if applicable)
    registry.registerPhysicsTerm("Resonance1", std::make_unique<Resonance1TermSourceN>(), "resonance_muge");
    // ... more resonance classes

    // Category 6: Astrophysical Systems (if applicable)
    registry.registerPhysicsTerm("System1", std::make_unique<System1TermSourceN>(), "system");
    // ... more system classes

    // Total: X classes registered
}
```

### Registration Categories

- `"graphics"` - Visualization/rendering infrastructure
- `"helper"` - Utility functions (step functions, gradients, etc.)
- `"gravity_wolfram"` - Core UQFF gravity terms (Ug1-4, Ubi, Um)
- `"unified_field"` - Complete unified field integration
- `"compressed_muge"` - MUGE compressed components
- `"resonance_muge"` - MUGE resonance components
- `"system"` - Astrophysical system templates
- `"utility"` - General utility classes
- `"fluid_dynamics"` - Navier-Stokes fluid solver components

---

## Phase 4: MAIN_1_CoAnQi.cpp Integration

### Edit Location 1: Add Includes (Currently Line ~22056)

```cpp
// After existing source file includes (source10, Source167-172, source6):

// SOURCEN MODULAR WOLFRAM INTEGRATION (X classes: brief description)
// Optional: Add complementary note if embedded version exists
#include "sourceN_wolfram.cpp"              // Base infrastructure
#include "sourceN_wolfram_graphics.cpp"     // Y graphics classes (if exists)
#include "sourceN_wolfram_physics.cpp"      // Z physics classes (if exists)
#include "sourceN_wolfram_compressed.cpp"   // W compressed classes (if exists)
#include "sourceN_wolfram_resonance.cpp"    // V resonance classes (if exists)
#include "sourceN_register.cpp"             // Registration function
```

**Important Notes:**

- Add includes in ascending numerical order (source4, source6, source7, etc.)
- Keep consistent formatting (alignment, comments)
- Include all category files before the register file
- Add blank line before next section

### Edit Location 2: Add Registration Call (Currently Line ~23237)

```cpp
// After existing registration calls, before "END BATCH" comment:

    // SOURCEN MODULAR WOLFRAM REGISTRATION (X classes: brief breakdown)
    registerSourceNPhysicsTerms(core.getRegistry());
    g_logger.log("SOURCEN modular complete: X classes registered (breakdown)", 1);
```

**Log Message Format:**

- Use exact class count
- Provide breakdown by category: "(Y graphics + Z physics)"
- Keep consistent with SOURCE6 format

---

## Phase 5: Documentation Updates

### File 1: SOURCE4_SOURCE6_COMBINED_ARCHITECTURE.md

Add new section for SOURCEN:

```markdown
## SOURCEN Framework (X Classes)

### File Structure
\`\`\`
sourceN.cpp                       (XXXX lines) - Description
sourceN_wolfram.cpp               Base infrastructure
sourceN_wolfram_[category].cpp    (XXXX lines) - Y [category] classes
sourceN_register.cpp              Registration function
\`\`\`

### Class Breakdown (X Classes)

#### **1. Category Name (Y classes)**
\`\`\`cpp
1. ClassName1Term                 - Brief description
2. ClassName2Term                 - Brief description
// ... list all classes
\`\`\`

### Key Features
- Feature 1 description
- Feature 2 description
- Unique capabilities

### Integration Synergies
- How SOURCEN complements existing frameworks
- Data flow interactions
- Visualization opportunities
```

### File 2: .github/copilot-instructions.md

Update architecture section:

```markdown
### SourceN Framework (X Classes)
- **sourceN.cpp**: Description
- **sourceN_wolfram_[category].cpp**: Y [category] classes
- **Key Feature**: Main unique capability
- **Integration Status**: ✅ Modular complete | ⏳ Pending | ❌ Not started
```

### File 3: README.md (Create if missing)

Update project overview with:

```markdown
### Modular Physics Frameworks
- **SOURCE4**: 47 classes (MUGE + Navier-Stokes + Self-Expanding)
- **SOURCE6**: 29 classes (Graphics Infrastructure + CelestialBody UQFF)
- **SOURCEN**: X classes (Brief description)
```

---

## Phase 6: Testing and Validation

### Step 1: Syntax Validation (Before Compilation)

```powershell
# Check for syntax errors without full compilation
Get-Content sourceN_*.cpp | Select-String -Pattern "class.*Term.*public PhysicsTerm"
# Expected output: List of all X class declarations
```

### Step 2: Compilation Test

```powershell
# Clean build to catch linking errors
Remove-Item -Recurse -Force build -ErrorAction SilentlyContinue
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release

# Expected output:
# - No compilation errors
# - MAIN_1_CoAnQi.exe created in build/Release/
```

### Step 3: Registration Verification

```powershell
# Run executable and check registration logs
.\build\Release\MAIN_1_CoAnQi.exe | Select-String -Pattern "SOURCEN modular complete"

# Expected output:
# "SOURCEN modular complete: X classes registered (breakdown)"
```

### Step 4: Runtime Testing

```cpp
// In interactive mode, test individual terms:
// 1. List registered terms
core.getRegistry().getTermNames();  // Should include "ClassName1_SourceN", etc.

// 2. Test compute function
auto term = core.getRegistry().getTerm("ClassName1_SourceN");
double result = term->compute(0.0, {{"param1", value1}, {"param2", value2}});
std::cout << "Result: " << result << std::endl;

// 3. Verify metadata
std::cout << "Source: " << term->getMetadata("source") << std::endl;  // "sourceN.cpp"
std::cout << "Type: " << term->getMetadata("type") << std::endl;      // Category
```

### Step 5: Performance Benchmarking

```cpp
// Time 1000 compute calls
auto start = std::chrono::high_resolution_clock::now();
for (int i = 0; i < 1000; ++i) {
    term->compute(i * 0.001, params);
}
auto end = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
std::cout << "Average compute time: " << duration.count() / 1000.0 << " µs" << std::endl;

// Expected: < 10 µs per call for typical physics terms
```

---

## Phase 7: Git Commit Standards

### Commit Message Format

```
feat(sourceN): Integrate SOURCEN modular framework (X classes)

- Add sourceN.cpp base implementation (YYYY lines)
- Add sourceN_wolfram_[category].cpp modular classes (Y classes)
- Add sourceN_register.cpp registration function
- Update MAIN_1_CoAnQi.cpp with includes and registration call
- Update SOURCE4_SOURCE6_COMBINED_ARCHITECTURE.md with SOURCEN section
- Update copilot-instructions.md with SOURCEN architecture

Total: X PhysicsTerm classes
- Y [category1] classes
- Z [category2] classes
- W [category3] classes

Key Features:
- Feature 1 brief description
- Feature 2 brief description
- Unique capability

Integration Status: ✅ Complete | ⏳ Pending | 🔄 In Progress
Compilation Status: ✅ Passed | ❌ Failed | ⏳ Not Tested
Registration Status: ✅ All X classes registered | ⏳ Pending
```

### Commit Checklist

- [ ] All source files added (4-6 files)
- [ ] MAIN_1_CoAnQi.cpp updated (2 edits)
- [ ] Documentation updated (3 files minimum)
- [ ] Compilation successful (no errors)
- [ ] Registration verified (log message confirmed)
- [ ] Commit message follows standard format
- [ ] No debug/temporary files committed (.vs/, build/, *.obj, etc.)

---

## Common Integration Patterns

### Pattern 1: Graphics + Physics Split (SOURCE6 Model)

```
sourceN_wolfram_graphics.cpp  → Visualization infrastructure
sourceN_wolfram_physics.cpp   → Core UQFF physics
sourceN_register.cpp          → Unified registration

Use when: Adding visualization capabilities alongside physics
Example: Rendering tools + astrophysical calculations
```

### Pattern 2: Compressed + Resonance Split (SOURCE4 Model)

```
sourceN_wolfram_compressed.cpp  → Simplified component breakdown
sourceN_wolfram_resonance.cpp   → Resonance-enhanced versions
sourceN_register.cpp            → Unified registration

Use when: Modular MUGE components with resonance variants
Example: Standard MUGE terms + resonance-enhanced versions
```

### Pattern 3: Monolithic Wolfram (Simple Model)

```
sourceN_wolfram.cpp  → All PhysicsTerm classes
sourceN_register.cpp → Registration function

Use when: < 20 classes, no natural category split
Example: Helper functions, utility classes, small frameworks
```

### Pattern 4: Multi-Category (Advanced Model)

```
sourceN_wolfram_category1.cpp
sourceN_wolfram_category2.cpp
sourceN_wolfram_category3.cpp
sourceN_register.cpp

Use when: 40+ classes with distinct category boundaries
Example: Graphics + Physics + Fluid Dynamics + Systems
```

---

## Troubleshooting Common Issues

### Issue 1: Compilation Error - "Undefined reference to registerSourceNPhysicsTerms"

**Cause**: Registration function not included or not visible at link time
**Solution**:

```cpp
// In sourceN_register.cpp, ensure function is NOT in anonymous namespace
void registerSourceNPhysicsTerms(PhysicsTermRegistry &registry)  // Correct
// NOT: namespace { void registerSourceNPhysicsTerms(...) }       // Wrong
```

### Issue 2: Registration Call Not Found in Logs

**Cause**: Registration call not added to registerInitialTerms()
**Solution**: Verify line ~23237 in MAIN_1_CoAnQi.cpp has:

```cpp
registerSourceNPhysicsTerms(core.getRegistry());
g_logger.log("SOURCEN modular complete: X classes registered", 1);
```

### Issue 3: Class Not Found in Registry at Runtime

**Cause**: Class not registered or name mismatch
**Solution**:

1. Check sourceN_register.cpp has registry.registerPhysicsTerm() call
2. Verify getName() matches registration name exactly
3. Check registry.getTermNames() output for actual registered name

### Issue 4: Duplicate Class Names Across Sources

**Cause**: Multiple sources use same class name (e.g., "UnifiedFieldUg1Term")
**Solution**: Add source suffix to class names:

```cpp
// Good: Unique names per source
UnifiedFieldUg1TermSource4
UniversalGravity1Source6Term

// Bad: Name collision
UnifiedFieldUg1Term  // Used by both SOURCE4 and SOURCE6
```

### Issue 5: Missing PhysicsTerm Base Class

**Cause**: sourceN_wolfram.cpp not included, or PhysicsTerm redefined
**Solution**:

1. Ensure sourceN_wolfram.cpp includes PhysicsTerm base class
2. Or rely on MAIN_1's existing PhysicsTerm definition
3. Use `class MyTerm : public PhysicsTerm` (public inheritance required)

### Issue 6: Slow Compilation After Adding Source

**Cause**: Large source files, circular includes, or template instantiation
**Solution**:

1. Split large files (> 2000 lines) into categories
2. Use forward declarations where possible
3. Minimize #include depth (include only what's needed)
4. Consider precompiled headers for common includes

---

## Performance Optimization Tips

### Tip 1: Minimize Runtime Parameter Lookups

```cpp
// Slow: Multiple map lookups
double compute(double t, const std::map<std::string, double> &params) const override {
    double result = params.at("a") * params.at("b") + params.at("c");  // 3 lookups
    return result;
}

// Fast: Single lookup with default fallback
double compute(double t, const std::map<std::string, double> &params) const override {
    auto it_a = params.find("a");
    auto it_b = params.find("b");
    auto it_c = params.find("c");
    double a = (it_a != params.end()) ? it_a->second : default_a;
    double b = (it_b != params.end()) ? it_b->second : default_b;
    double c = (it_c != params.end()) ? it_c->second : default_c;
    return a * b + c;
}
```

### Tip 2: Cache Expensive Calculations

```cpp
class OptimizedTerm : public PhysicsTerm {
private:
    mutable double cached_value;
    mutable double cached_t;
    
public:
    double compute(double t, const std::map<std::string, double> &params) const override {
        // Cache results for repeated t values
        if (t == cached_t) return cached_value;
        
        cached_value = /* expensive calculation */;
        cached_t = t;
        return cached_value;
    }
};
```

### Tip 3: Use constexpr for Compile-Time Constants

```cpp
class FastTerm : public PhysicsTerm {
private:
    static constexpr double PI = 3.141592653589793;
    static constexpr double G = 6.67430e-11;
    static constexpr double c_light = 299792458.0;
    
    // Compile-time calculation
    static constexpr double G_c2 = G / (c_light * c_light);  // Precalculated
};
```

### Tip 4: Prefer Member Variables Over Runtime Extraction

```cpp
// Slow: Extract from params every compute() call
double compute(double t, const std::map<std::string, double> &params) const override {
    double mass = params.at("mass");  // Runtime lookup
    return mass * t;
}

// Fast: Store as member, initialize in constructor
class FastTerm : public PhysicsTerm {
private:
    double mass;  // Member variable
public:
    FastTerm(double m) : mass(m) {}
    double compute(double t, const std::map<std::string, double> &) const override {
        return mass * t;  // Direct access
    }
};
```

---

## Advanced Integration Scenarios

### Scenario 1: Self-Expanding Framework (SOURCE4 Model)

If SOURCEN should generate PhysicsTerms at runtime:

```cpp
class SourceNExpander {
public:
    void generateTermsFromData(PhysicsTermRegistry &registry, const DataSource &data) {
        // Read data (file, database, API)
        auto dataset = data.load();
        
        // Generate terms dynamically
        for (const auto &entry : dataset) {
            auto term = std::make_unique<GeneratedTerm>(entry.params);
            term->setMetadata("source", "sourceN_generated");
            term->setMetadata("type", "runtime_generated");
            registry.registerPhysicsTerm(entry.name, std::move(term), "generated");
        }
    }
};

// In registerSourceNPhysicsTerms():
void registerSourceNPhysicsTerms(PhysicsTermRegistry &registry) {
    // Static terms first
    registry.registerPhysicsTerm("StaticTerm1", ...);
    
    // Dynamic generation
    SourceNExpander expander;
    expander.generateTermsFromData(registry, dataSource);
}
```

### Scenario 2: CelestialBody Integration (SOURCE6 Model)

If SOURCEN uses CelestialBody struct:

```cpp
// In sourceN.cpp:
struct CelestialBodySourceN {
    double mass, radius, temperature, /* ... */;
    
    // JSON serialization
    void to_json(nlohmann::json &j) const { /* ... */ }
    void from_json(const nlohmann::json &j) { /* ... */ }
};

// In sourceN_wolfram_physics.cpp:
class SystemTermSourceN : public PhysicsTerm {
private:
    CelestialBodySourceN body;
public:
    SystemTermSourceN(const CelestialBodySourceN &b) : body(b) {}
    // Use body.mass, body.radius, etc. in compute()
};
```

### Scenario 3: Graphics Interoperability (SOURCE6 Model)

If SOURCEN needs to render SOURCE4/SOURCE6 physics:

```cpp
// In sourceN_wolfram_graphics.cpp:
class PhysicsFieldRenderer : public PhysicsTerm {
public:
    double compute(double t, const std::map<std::string, double> &params) const override {
        // Get physics term from another source
        auto physics_term = registry->getTerm("UnifiedFieldUg1_Source4");
        
        // Sample field at multiple points
        std::vector<double> field_values;
        for (int i = 0; i < 100; ++i) {
            std::map<std::string, double> sample_params = params;
            sample_params["x"] = i * 0.1;
            field_values.push_back(physics_term->compute(t, sample_params));
        }
        
        // Render to OpenGL/Vulkan (simplified)
        renderFieldToGPU(field_values);
        
        // Return rendering metric
        return field_values.size();
    }
};
```

---

## Version History

| Version | Date | Changes | Author |
|---------|------|---------|--------|
| 1.0 | 2025-12-10 | Initial creation based on SOURCE4/SOURCE6 patterns | AethericPropulsion Team |

---

## Quick Reference Card

```
┌─────────────────────────────────────────────────────────────────────┐
│  SOURCEN INTEGRATION QUICK REFERENCE                                │
├─────────────────────────────────────────────────────────────────────┤
│  FILES TO CREATE (4-6):                                             │
│    □ sourceN.cpp (base implementation)                              │
│    □ sourceN_wolfram.cpp (infrastructure)                           │
│    □ sourceN_wolfram_[category].cpp (classes)                       │
│    □ sourceN_register.cpp (registration)                            │
├─────────────────────────────────────────────────────────────────────┤
│  MAIN_1 EDITS (2):                                                  │
│    Line ~22056: Add #include "sourceN_*.cpp" statements             │
│    Line ~23237: Add registerSourceNPhysicsTerms() call              │
├─────────────────────────────────────────────────────────────────────┤
│  DOCUMENTATION UPDATES (3):                                         │
│    □ SOURCE4_SOURCE6_COMBINED_ARCHITECTURE.md (add SOURCEN section)│
│    □ .github/copilot-instructions.md (add architecture notes)       │
│    □ README.md (add to framework list)                              │
├─────────────────────────────────────────────────────────────────────┤
│  TESTING (4 steps):                                                 │
│    1. cmake --build build --config Release                          │
│    2. Check for "SOURCEN modular complete" in logs                  │
│    3. Verify registry.getTermNames() includes SOURCEN classes       │
│    4. Test individual term->compute() calls                         │
├─────────────────────────────────────────────────────────────────────┤
│  COMMIT FORMAT:                                                     │
│    feat(sourceN): Integrate SOURCEN modular framework (X classes)   │
│    - List of changes                                                │
│    - Key features                                                   │
│    - Integration status                                             │
└─────────────────────────────────────────────────────────────────────┘
```

---

**END OF REFERENCE GUIDE**

This document provides a complete, standardized procedure for integrating future source files into the AethericPropulsion UQFF framework. Follow this guide to maintain consistency across all SOURCE7+ integrations.
