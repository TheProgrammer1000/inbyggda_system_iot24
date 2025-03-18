idf_component_register(SRCS "main.cpp"
                    INCLUDE_DIRS ".")

# Set a unique ULP binary name using the component name
set(ulp_app_name ulp_${COMPONENT_NAME})

# Specify the ULP source file (note the relative path)
set(ulp_sources "ulp/ulp.c")
set(ulp_exp_dep_srcs "ulp/ulp.c")

# Embed the ULP binary into your component.
ulp_embed_binary(${ulp_app_name} "${ulp_sources}" "${ulp_exp_dep_srcs}")