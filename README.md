Licensed Under Creative Commons No Derivatives Non Commerical Open Source

With sudo nano
Save pulse_extract.sh to a directory
Save nano64 pulse file nano64_repo.pulse to same directory
type sudo +x pulse_extract.sh
enter password
type ./pulse_extract.sh
presto

Other pulse methods work

base64 bundle.pulse > bundle.pulse.b64

[Convert]::ToBase64String([IO.File]::ReadAllBytes("bundle.pulse")) > bundle.pulse.b64
REPLACE_WITH_YOUR_BASE64_PULSE_DATA
sh pulse_extract.sh
