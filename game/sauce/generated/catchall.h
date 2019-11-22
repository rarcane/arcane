typedef struct Foo Foo;
struct Foo
{
i32 integer_value;
b32 boolean_value;
f32 float_value;
void *pointer;
};

void PrintFoo(Foo *);
