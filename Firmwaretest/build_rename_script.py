Import("env")

# Fetch all definition build flags ('-D')
my_flags = env.ParseFlags(env['BUILD_FLAGS'])
cppdefines = my_flags.get("CPPDEFINES")

# Get the firmware version number and matching hardware version tag
for define in cppdefines:
	if define[0] == 'FW_VERSION':
		version = define[1]
		version = version.strip('\"')
		fw_version = version
	if define[0] == 'HW_TAG':
		hw_tag = define[1]
	if define[0] == 'MODEL':
		model = define[1]

# Check if handle alpha and beta version strings
if version.find('beta') != -1  or version.find('alpha') != -1:
	version = version.split('-', 1)
	version = version[0] + '.' + hw_tag + '-' + version[1]
else:
	version = version + '.' + hw_tag

version = model + '_v' + version

print('Building for version: ' + version)
env.Replace(PROGNAME=version)

def post_build_action(source, target, env):
	import shutil
	import os
	firmware_path = os.path.join(env['PROJECT_DIR'], 'build_outputs/' + fw_version + '/' + model)
	print(firmware_path)
	if not os.path.exists(firmware_path):
		os.makedirs(firmware_path)
	print(target[0].path)
	shutil.copy(target[0].path, firmware_path)

env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", post_build_action)