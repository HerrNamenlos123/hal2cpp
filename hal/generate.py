import json
import sys

# net spindle-on <= spindle.0.on
# net spindle-cmd-rpm     <= spindle.0.speed-out
# net spindle-cmd-rpm-abs <= spindle.0.speed-out-abs
# net spindle-cmd-rps     <= spindle.0.speed-out-rps
# net spindle-cmd-rps-abs <= spindle.0.speed-out-rps-abs
# net spindle-at-speed    => spindle.0.at-speed
# net spindle-cw <= spindle.0.forward

# joint.0.motor-pos-cmd
# joint.0.motor-pos-fb
# joint.0.amp-enable-out

# net estop-out <= iocontrol.0.user-enable-out
# net estop-out => iocontrol.0.emc-enable-in

with open(sys.argv[1], 'w') as out:
    out.write("# This file was generated by hal2cpp. The original Stepconf file was overwritten by hal2cpp.\n")
    out.write("# If you rerun Stepconf, make sure to also rerun hal2cpp, to overwrite this file again.\n")
    out.write("# DO NOT MAKE CHANGES TO THIS FILE! They will be lost!\n")
    out.write("\n")
    out.write("loadrt [KINS]KINEMATICS\n")
    out.write("loadrt [EMCMOT]EMCMOT base_period_nsec=[EMCMOT]BASE_PERIOD servo_period_nsec=[EMCMOT]SERVO_PERIOD num_joints=[KINS]JOINTS\n")
    out.write("addf motion-command-handler servo-thread\n")
    out.write("addf motion-controller servo-thread\n")
    out.write("loadrt hal2cpp\n")
    out.write("addf hal2cpp.0 servo-thread\n")

    out.write("\n")

    with open('config.json', 'r') as config:
        j = json.loads(config.read())

        for pin in j["pins"]:
            net = pin["net"]
            for input in pin["from"]:
                out.write(f"net {net} <= {input}\n")
            out.write(f"net {net} => {pin['to']}\n")
            out.write("\n")

        for code in j["additional_code"]:
            out.write(code)
