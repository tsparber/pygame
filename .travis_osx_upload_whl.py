""" Uploads wheels to pypi from travisci.

The commit requires an UPLOAD line.
"""
import glob
import os
import subprocess
import sys


def write_config():
    """
    # Set these inside travis. Note, it is on a per repo basis.
    # https://docs.travis-ci.com/user/environment-variables/#Encrypting-Variables-Using-a-Public-Key
    # travis encrypt PYPI_USERNAME=super_secret --add
    # travis encrypt PYPI_PASSWD=super_secret --add
    """
    if not os.environ.get('PYPI_USERNAME', None):
        return

    username = os.environ['PYPI_USERNAME']
    password = os.environ['PYPI_PASSWD']

    pypirc_template = """\
[distutils]
index-servers =
    pypi
[pypi]
repository: https://upload.pypi.io/legacy/
username: {username}
password: {password}
""".format(username=username, password=password)

    with open('pypirc', 'w') as afile:
        afile.write(pypirc_template)

if '--write-config' in sys.argv:
    write_config()
    sys.exit(0)
else:
    if '--no-config' not in sys.argv:
        write_config()

# There should be exactly one .whl
filenames = glob.glob('dist/*.whl')

print('Calling transfer.sh to upload...')
try:
    for filename in filenames:
        cmd = ['curl', '--upload-file', filename, 'https://transfer.sh/' + os.path.basename(filename)]
        process = subprocess.Popen(cmd, stdout=subprocess.PIPE)
        out, err = process.communicate()
        out = out.decode()
        print(out)
        id = out.split('/')[3]
        # redirector to keep filename, as transfer.sh replaces underscores with dashes
        print('https://sparber.net/r/%s/%s' % (id, os.path.basename(filename)))
except Exception as e:
    print('error uploading to transfer.sh: %s' % str(e))


# if --no-git we do not check with git if an upload is needed.
do_git_check = '--no-git' not in sys.argv
if do_git_check:
    commit = subprocess.check_output(['git', 'log', '-1'])
    print(commit)
    if b'UPLOAD' not in commit:
        print('Not uploading')
        sys.exit(0)


print('Calling twine to upload...')
try:
    for filename in filenames:
        cmd = ['twine', 'upload', '--config-file', 'pypirc', filename]
        print(' '.join(cmd))
        subprocess.check_call(cmd)
except:
    print('is twine installed?')
finally:
    os.unlink('pypirc')
