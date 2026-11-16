# SYS 01 parameter catalogue

generated from `host/catalogue.hpp`; do not edit by hand.

| stable ID | label | normalized default | steps |
|---|---|---|---|
| `param_flux` | FLUX | 0.50 | 0 |
| `param_bias` | BIAS | 0.50 | 0 |
| `transport_drive` | DRIVE | 0.00 | 2 |
| `subregister_state` | STATE | 0.00 | 2 |
| `engine_state` | DOMAIN | 0.00 | 3 |
| `output_trim` | OUTPUT | 0.75 | 0 |
| `bypass` | BYPASS | 0.00 | 2 |

steps 0 means continuous. all parameters are automatable. DOMAIN uses 0 / 0.5 / 1; binary parameters use 0 / 1. OUTPUT maps -60..+20 dB. STATE is shared across domains. schema 1.

factory studies (parameter-only; empty captured tape on recall):

- initial / paused
- 01 / magnetic pulse
- 02 / cellular walls
- 03 / forward tape
