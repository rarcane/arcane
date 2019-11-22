void PrintFoo(Foo *object)
{

printf("{ ");
printf("%i, ", object->integer_value);
printf("%s, ", object->boolean_value ? "true" : "false");
printf("%f, ", object->float_value);
printf("}");

}

