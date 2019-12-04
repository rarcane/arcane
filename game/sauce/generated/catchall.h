typedef enum EntityType EntityType;
enum EntityType
{
ENTITY_undefined,
ENTITY_character,
ENTITY_monster,
ENTITY_animal,
ENTITY_item,
ENTITY_storage,
ENTITY_resource,
ENTITY_scenic,
ENTITY_ground,
ENTITY_MAX,
};
static char *GetEntityTypeName(EntityType type);

typedef struct Foo Foo;
struct Foo
{
i32 integer_value;
b32 boolean_value;
f32 float_value;
void *pointer;
};

void PrintFoo(Foo *);
