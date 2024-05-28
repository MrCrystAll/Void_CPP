import sys
import json
import traceback

import socket


# =======================
# Example implementation of render receiver, using RocketSimVis
# =======================

# Send to RocketSimVis
UDP_IP = "127.0.0.1"
UDP_PORT = 9273

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
 
def write_physobj(physobj):
	j = {}
	
	j['pos'] = physobj["pos"]
	j['forward'] = physobj["forward"]
	j['up'] = physobj["up"]
	j['vel'] = physobj["vel"]
	j['ang_vel'] = physobj["ang_vel"]
	
	return j

def write_car(player):
	j = {}
	
	j['team_num'] = int(player["team_num"])
	j['phys'] = write_physobj(player["phys"])
	
	j['boost_amount'] = player["boost_amount"] * 100
	j['on_ground'] = bool(player["on_ground"])
	j['is_demoed'] = bool(player["is_demoed"])
	j['has_flip'] = bool(player["has_flip"])

	return j

# RocketSimVis needs to know where the boost pads are
# This is the order of boost locations from RLGym
BOOST_LOCATIONS = ( 
	(0, -4240, 70), (-1792, -4184, 70), (1792, -4184, 70), (-3072, -4096, 73), (3072, -4096, 73), 
	(- 940, -3308, 70), (940, -3308, 70), (0, -2816, 70), (-3584, -2484, 70), (3584, -2484, 70), 
	(-1788, -2300, 70), (1788, -2300, 70), (-2048, -1036, 70), (0, -1024, 70), (2048, -1036, 70), 
	(-3584, 0, 73), (-1024, 0, 70), (1024, 0, 70), (3584, 0, 73), (-2048, 1036, 70), (0, 1024, 70), 
	(2048, 1036, 70), (-1788, 2300, 70), (1788, 2300, 70), (-3584, 2484, 70), (3584, 2484, 70), 
	(0, 2816, 70), (- 940, 3310, 70), (940, 3308, 70), (-3072, 4096, 73), (3072, 4096, 73), 
	(-1792, 4184, 70), (1792, 4184, 70), (0, 4240, 70)
)

def send_state_to_rocketsimvis(gs):
	j = {}
	
	# Send ball
	j['ball_phys'] = write_physobj(gs["ball"])
	
	# Send cars
	j['cars'] = []
	for player in gs["players"]:
		j['cars'].append(write_car(player))

	# Send boost pad states
	j['boost_pad_states'] = gs["boost_pads"]

	sock.sendto(json.dumps(j).encode('utf-8'), (UDP_IP, UDP_PORT))

def render_state(state_json_str):
	j = json.loads(state_json_str)
	try:
		if 'state' in j:
			send_state_to_rocketsimvis(j['state'])
		else:
			send_state_to_rocketsimvis(j)
	except Exception as err:
		print("Exception while sending data:")
		traceback.print_exc()
