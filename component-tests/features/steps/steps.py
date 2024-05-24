from behave import *
from utility import Playback

@step("a IRC server connection: {ip} {port}")
def step_impl(context, ip, port):
    context.ip = ip
    context.port = port

@step("replay the trace {filename}")
def step_impl(context, filename):
    filename = "./features/test-data/" + filename
    playback = Playback(filename, context.ip, int(context.port))
    validate_responses = True
    if context.config.userdata.get("validate_responses", "true") == "false":
        validate_responses = False
    playback.run(validate_responses)
