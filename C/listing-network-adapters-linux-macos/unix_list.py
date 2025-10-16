import psutil

addr_adapters = psutil.net_if_addrs()

for interface_name, interface_addresses in addr_adapters.items():
    print(f"Interface: {interface_name}")
    for address in interface_addresses:
        if address.family == 2 or address.family == 30:
            print(f"  Address Family: {address.family}")
            print(f"  Address: {address.address}")
            print(f"  Netmask: {address.netmask}")
            print(f"  Broadcast: {address.broadcast}")
            print()

