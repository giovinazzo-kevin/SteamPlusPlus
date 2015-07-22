FriendChatMsg_t = 805

function onFriendChatMsg(cubParam, pubParam)
	spp.print("You received or sent a message!\n");
end

function main(argc, argv)
	spp.print("Hello from " .. argv[0] .. "!\nThis script tests every function that SPP binds to the LUA environment.\n\n")

	if argc == 1 then
		spp.info("Please pass more than one argument to the script. It can be anything you want.\n\n")
		return 1
	end

	spp.print("This script was passed " .. math.floor(argc) .. " arguments.\n")
	spp.print("Those arguments are: \n")
	for i = 0, argc - 1 do
		spp.print("\t" .. argv[i] .. "\n")
	end
	spp.print("\n")

	nTestsFailed = 0

	spp.print("[TEST] Registering a callback.\nExpected return value: 0\n")
	ret = spp.registerCallback(FriendChatMsg_t, "onFriendChatMsg")
	spp.info("Returned value: " .. ret .. "\n")
	if ret ~= 0 then 
		spp.err("Unexpected return value.\n")
		nTestsFailed = nTestsFailed + 1
	end
	--[[
	spp.print("[TEST] Unregistering a callback.\nExpected return value: 0\n")
	ret = spp.unregisterCallback(705)
	spp.info("Returned value: " .. ret .. "\n")
	if ret ~= 0 then 
		spp.err("Unexpected return value.\n")
		nTestsFailed = nTestsFailed + 1
	end
	]]--

	spp.print("\n")

	return nTestsFailed
end