# Lima MUD LPC Coding Standards

## Overview
This document defines coding standards and best practices for LPC development in the Lima MUD project. These standards help prevent common compilation errors and ensure code consistency.

## LPC-Specific Requirements

### Variable Declarations
**Rule**: All variable declarations must be at the beginning of a function or block.

**Correct**:
```lpc
void my_function() {
   int x;
   string y;
   object z;
   
   x = 5;
   y = "hello";
   z = find_object("/std/object");
}
```

**Incorrect**:
```lpc
void my_function() {
   int x = 5;
   string y = "hello";  // Error: declaration in middle of function
   object z = find_object("/std/object");
}
```

### Function Calls in Modules
**Rule**: When calling functions that come from the inheriting object (not the module itself), use `this_object()->` prefix.

**Correct**:
```lpc
// In a module that gets inherited by objects
return "This " + this_object()->query_id()[0] + " doesn't fit.\n";
```

**Incorrect**:
```lpc
// This will fail when compiling the module in isolation
return "This " + query_id()[0] + " doesn't fit.\n";
```

### Inheritance Order
**Rule**: Inherit base classes before modules to ensure proper function resolution.

**Correct**:
```lpc
inherit OBJ;
inherit M_WEARABLE;
inherit M_GETTABLE;
```

**Incorrect**:
```lpc
inherit M_WEARABLE;
inherit OBJ;  // May cause function resolution issues
inherit M_GETTABLE;
```

### Forward Declarations
**Rule**: When functions are called before they're defined, add forward declarations at the top of the file.

**Correct**:
```lpc
// Forward declarations
void setup_race_body(string race);
string query_race_name();
int query_health();

// Function implementations
void setup_race_body(string race) {
   // implementation
}
```

### Function Parameter Handling
**Rule**: Use `varargs` for optional parameters and handle them properly.

**Correct**:
```lpc
varargs int query_health(string limb) {
   if (!limb) {
      // Handle no parameter case
      return current_health;
   }
   // Handle with parameter case
   return query_limb_health(limb);
}
```

## Code Organization

### File Structure
1. **Headers and includes** at the top
2. **Forward declarations** (if needed)
3. **Global variables** and constants
4. **Function implementations**
5. **Helper functions** at the bottom

### Function Documentation
Use the standard Lima documentation format:

```lpc
//: FUNCTION function_name
// Return type function_name(parameters);
// Brief description of what the function does.
```

### Error Handling
**Rule**: Always check for null objects and valid parameters before using them.

**Correct**:
```lpc
if (object && object->query_health) {
   health = object->query_health();
}
```

## Common Pitfalls to Avoid

### 1. Missing Semicolons
LPC requires semicolons after statements, but not after function definitions.

### 2. Array/Mapping Syntax
**Correct**:
```lpc
string *array = ({"item1", "item2"});
mapping map = (["key": "value"]);
```

**Incorrect**:
```lpc
string *array = {"item1", "item2"};  // Missing parentheses
mapping map = ["key": "value"];      // Missing parentheses
```

### 3. String Concatenation
**Correct**:
```lpc
string result = "Hello " + name + "!";
```

**Incorrect**:
```lpc
string result = "Hello " name "!";  // Missing + operators
```

### 4. Function Calls with Arrays
**Correct**:
```lpc
function_name(({"param1", "param2"}));
```

**Incorrect**:
```lpc
function_name({"param1", "param2"});  // Missing outer parentheses
```

## Module-Specific Guidelines

### M_WEARABLE Module
- Always use `this_object()->` for functions from inheriting objects
- Declare all variables at function start
- Use `varargs` for optional parameters

### Race System
- Define body parts with slot and hit_weight properties
- Include race_type for equipment compatibility
- Use consistent naming conventions

### Health System
- Use global HP pool (no per-limb health)
- Maintain compatibility with existing function signatures
- Handle limb parameters gracefully (ignore them)

## Testing Guidelines

### Before Committing
1. Run `update` command on modified files
2. Test basic functionality in-game
3. Verify no compilation errors
4. Check for memory leaks or crashes

### Common Test Commands
```bash
update std/modules/m_wearable.c
update std/adversary/health/race_body.c
```

## File Naming Conventions

- **Modules**: `m_*.c` (e.g., `m_wearable.c`)
- **Base classes**: descriptive names (e.g., `armour.c`, `weapon.c`)
- **Race files**: race name (e.g., `human.c`, `squid.c`)
- **Daemons**: `*_d.c` (e.g., `body_d.c`)

## Documentation Standards

### Function Headers
```lpc
//: FUNCTION query_health
// int query_health(string limb);
// Returns the current health of the specified limb or overall health if no limb specified.
```

### Class Documentation
```lpc
//: CLASS wear_info
// Contains information about what is being worn on a body part.
//   primary: The primary item worn on this part
//   secondary: The secondary (under) item worn on this part
//   others: Array of other items covering this part
```

## Performance Considerations

### Memory Management
- Avoid creating unnecessary objects
- Use `nosave` for variables that don't need persistence
- Clean up arrays and mappings when no longer needed

### Function Efficiency
- Cache frequently accessed values
- Use `member_array()` instead of loops when possible
- Minimize string concatenations in loops

## Security Guidelines

### Input Validation
- Always validate user input
- Check object existence before calling methods
- Use `owner()` checks for player-owned objects

### File Access
- Use `find_object()` instead of `load_object()` when possible
- Validate file paths before accessing
- Handle file not found errors gracefully

---

**Remember**: When in doubt, follow the existing codebase patterns and conventions. Consistency is more important than perfection. 