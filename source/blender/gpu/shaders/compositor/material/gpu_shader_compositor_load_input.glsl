void load_input_float(sampler2D input_sampler, out float value)
{
  ivec2 xy = ivec2(gl_GlobalInvocationID.xy);
  value = texelFetch(input_sampler, xy, 0).x;
}

void load_input_vector(sampler2D input_sampler, out vec3 vector)
{
  ivec2 xy = ivec2(gl_GlobalInvocationID.xy);
  vector = texelFetch(input_sampler, xy, 0).xyz;
}

void load_input_color(sampler2D input_sampler, out vec4 color)
{
  ivec2 xy = ivec2(gl_GlobalInvocationID.xy);
  color = texelFetch(input_sampler, xy, 0);
}