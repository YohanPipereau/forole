I did not want to implement creation of tap interfaces with VAPI you can use:

```
vppctl create tap
vppctl show int
```

Then you read the sw_if_index of the newly created tap interface.

You run with appropriate priviledges to use VPP API:

```
./remove_tap_by_sw_index $index_of_tap
```
