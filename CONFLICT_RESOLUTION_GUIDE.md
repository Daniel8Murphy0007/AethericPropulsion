# Quick Conflict Resolution Guide
**For**: AethericPropulsion Repository  
**Purpose**: Step-by-step guide for resolving merge conflicts when they occur

---

## 🚨 When Do Conflicts Occur?

Merge conflicts happen when:
- Two branches modify the same lines in a file differently
- One branch modifies a file while another deletes it
- Multiple branches reorganize code in incompatible ways

---

## 🔍 Step 1: Detect Conflicts

### Check if you have conflicts:
```bash
# Check repository status
git status

# Look for "Unmerged paths" section
# Conflicted files will be listed with "both modified" status
```

### Identify conflicted files:
```bash
# List all unmerged files
git diff --name-only --diff-filter=U

# See detailed conflict information
git diff
```

---

## 📖 Step 2: Understand Conflict Markers

When you open a conflicted file, you'll see markers like this:

```cpp
<<<<<<< HEAD (Current Change)
// Your current branch's code
int result = calculate_uqff_field(x, y);
=======
// Incoming branch's code  
int result = compute_quantum_field(x, y);
>>>>>>> feature-branch (Incoming Change)
```

**Markers explained:**
- `<<<<<<< HEAD`: Start of your current branch's version
- `=======`: Separator between versions
- `>>>>>>> branch-name`: End of incoming branch's version

---

## ✏️ Step 3: Resolve the Conflict

### Option A: Manual Resolution (Recommended for small conflicts)

1. **Open the conflicted file** in your editor (VS Code, vim, etc.)

2. **Decide what to keep:**
   - Keep current changes only
   - Keep incoming changes only  
   - Keep both (merge them together)
   - Write new code that combines both approaches

3. **Edit the file** to keep what you want and remove conflict markers

4. **Remove all conflict markers** (`<<<<<<<`, `=======`, `>>>>>>>`)

**Example Resolution:**
```cpp
// BEFORE (conflicted)
<<<<<<< HEAD
int result = calculate_uqff_field(x, y);
=======
int result = compute_quantum_field(x, y);
>>>>>>> feature-branch

// AFTER (resolved - kept both, renamed appropriately)
int uqff_result = calculate_uqff_field(x, y);
int quantum_result = compute_quantum_field(x, y);
```

### Option B: Use Visual Tools

#### VS Code (Built-in)
1. Open conflicted file
2. Click "Accept Current Change", "Accept Incoming Change", or "Accept Both Changes"
3. Edit as needed
4. Save file

#### Git Mergetool
```bash
# Configure merge tool (one time setup)
git config --global merge.tool meld  # or kdiff3, vimdiff, etc.

# Launch merge tool
git mergetool

# Follow tool's UI to resolve conflicts
```

### Option C: Accept One Side Completely

```bash
# Keep your version (current branch)
git checkout --ours path/to/file.cpp

# Keep their version (incoming branch)
git checkout --theirs path/to/file.cpp
```

---

## ✅ Step 4: Mark as Resolved

After resolving conflicts in a file:

```bash
# Stage the resolved file
git add path/to/resolved-file.cpp

# Check status to see remaining conflicts
git status
```

**Repeat for all conflicted files** until `git status` shows no unmerged paths.

---

## 🎯 Step 5: Complete the Merge

### If you were doing a merge:
```bash
# Complete the merge
git merge --continue

# Or if you want to abort
git merge --abort
```

### If you were doing a rebase:
```bash
# Continue the rebase
git rebase --continue

# Or if you want to abort
git rebase --abort
```

### If you were doing a cherry-pick:
```bash
# Continue the cherry-pick
git cherry-pick --continue

# Or abort
git cherry-pick --abort
```

---

## 🧪 Step 6: Test Your Resolution

**Always test after resolving conflicts!**

```bash
# For C++ projects (like this one)
mkdir -p build && cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release

# Run tests if available
ctest --output-on-failure

# Manual testing
./build/Release/uqff_calculator.exe
```

---

## 📋 Common Scenarios for This Repository

### Scenario 1: Struct Definition Conflicts (Like Previous Resolution)

**Problem**: Multiple source files define similar structs (e.g., DPMVars)

**Solution**:
1. Rename structs with file-specific suffixes
2. Update all references in the file
3. Keep base definition in header file

**Example**:
```cpp
// source168.cpp
struct DPMVars_S168 { /* ... */ };

// Source167.cpp  
struct DPMVars_S167 { /* ... */ };

// UQFFBuoyancy.h (shared)
struct DPMVars { /* ... */ };
```

### Scenario 2: Include Statement Conflicts

**Problem**: Different branches add different headers

**Solution**: Keep both includes (unless they're redundant)

```cpp
<<<<<<< HEAD
#include "observational_systems_config.h"
=======
#include "uqff_tracing.h"
>>>>>>> feature

// RESOLVED: Keep both
#include "observational_systems_config.h"
#include "uqff_tracing.h"
```

### Scenario 3: Function Implementation Conflicts

**Problem**: Same function modified differently

**Solution**: 
1. Compare both implementations
2. Understand why each change was made
3. Merge logic if both are needed
4. Choose better implementation if they're alternatives

### Scenario 4: CMakeLists.txt Conflicts

**Problem**: Different dependencies or settings added

**Solution**: Merge both sets of changes

```cmake
# Keep both dependency additions
find_package(Qt6 COMPONENTS Core Widgets)
find_package(OpenCV REQUIRED)

# Combine target link libraries
target_link_libraries(uqff_calculator
    Qt6::Core Qt6::Widgets  # from one branch
    opencv_core opencv_imgproc  # from another branch
)
```

---

## 🛡️ Prevention Tips

### 1. Keep Branches Up to Date
```bash
# Regularly update your feature branch from main
git checkout your-feature-branch
git fetch origin
git merge origin/main  # or git rebase origin/main
```

### 2. Communicate with Team
- Announce when working on core files
- Create focused, short-lived branches
- Break large features into smaller PRs

### 3. Use Feature Flags
Instead of long-lived branches, use feature flags:
```cpp
#ifdef FEATURE_NEW_QUANTUM_CALC
    result = new_quantum_calculation();
#else
    result = legacy_calculation();
#endif
```

### 4. Modular Architecture
This project uses modular physics classes - keep changes isolated:
- New PhysicsTerm classes in separate files
- Use plugin architecture (source4, source6 modules)
- Avoid modifying core MAIN_1_CoAnQi.cpp when possible

---

## 🆘 When to Ask for Help

Ask for help if:
- ❌ You don't understand what both versions do
- ❌ Conflict involves complex physics calculations
- ❌ Changes affect critical UQFF framework code
- ❌ Multiple files have interdependent conflicts
- ❌ Tests fail after resolution and you don't know why

**Who to ask**: Repository maintainer or team lead

---

## 🔧 Advanced: Conflict Resolution Strategies

### Strategy 1: Three-Way Merge Understanding

```bash
# Find the common ancestor (merge base)
git merge-base branch1 branch2

# See what changed in each branch
git diff merge-base..branch1 path/to/file
git diff merge-base..branch2 path/to/file
```

### Strategy 2: Interactive Rebase for Clean History

```bash
# Rebase your branch on updated main
git checkout feature-branch
git fetch origin
git rebase origin/main

# Resolve conflicts one commit at a time
# Each commit can be resolved independently
```

### Strategy 3: Temporary Branch for Testing

```bash
# Create test branch to try resolution
git checkout -b test-merge-resolution
git merge feature-branch  # Try resolving here
# Test thoroughly
# If good, apply same resolution to actual branch
```

---

## 📚 Additional Resources

### Git Documentation
- [Git SCM - Basic Merge Conflicts](https://git-scm.com/book/en/v2/Git-Branching-Basic-Branching-and-Merging)
- [GitHub Docs - Resolving Conflicts](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/addressing-merge-conflicts)

### Tools
- **VS Code**: Built-in merge conflict resolver
- **GitKraken**: Visual merge conflict tool
- **Meld**: Open source diff/merge tool
- **Beyond Compare**: Professional comparison tool

### Repository Specific
- See: `MERGE_CONFLICT_ANALYSIS.md` for current conflict status
- See: `BUILD_INSTRUCTIONS.md` for testing after resolution
- See: `SOURCE4_INTEGRATION_COMPLETE.md` for modular architecture

---

## 🎓 Quick Reference Commands

```bash
# Conflict detection
git status
git diff --name-only --diff-filter=U

# Accept one side
git checkout --ours <file>    # Keep your changes
git checkout --theirs <file>  # Keep incoming changes

# Mark resolved
git add <file>

# Continue operation
git merge --continue
git rebase --continue
git cherry-pick --continue

# Abort operation
git merge --abort
git rebase --abort
git cherry-pick --abort

# Preview merge
git merge-tree $(git merge-base branch1 branch2) branch1 branch2
```

---

**Last Updated**: December 22, 2025  
**For Repository**: AethericPropulsion  
**Maintained by**: Development Team
