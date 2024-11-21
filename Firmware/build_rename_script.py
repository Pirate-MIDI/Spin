Import("env")

# Fetch all definition build flags ('-D')
my_flags = env.ParseFlags(env['BUILD_FLAGS'])
cppdefines = my_flags.get("CPPDEFINES")

# Get the firmware version number and matching hardware version tag
for define in cppdefines:
    if define[0] == 'FW_VERSION':
        version = define[1]
    if define[0] == 'HW_TAG':
        hw_tag = define[1]

print("Building for version: v" + version + '.' + hw_tag)
if "CONFIG_MAIN" in cppdefines:
    env.Replace(PROGNAME="spin_v%s" % version + '.' + hw_tag)

if "CONFIG_DIAGNOSTIC" in cppdefines:
    env.Replace(PROGNAME="spin_diagnostic_v%s" % version + '.' + hw_tag)
