
without smep feature enable test log:

zswan@zswan-developer:~/testapp/smep$ cat withoutsmep.log
[   86.107293] BUG: unable to handle kernel paging request at 00000000043ff954
[   86.107553] IP: [<00000000043ff954>] 0x43ff954
[   86.107718] PGD da632067 PUD db0ac067 PMD 0
[   86.107886] Oops: 0010 [#1] SMP
[   86.108015] Modules linked in: smep_test_driver(OE) bnep rfcomm bluetooth snd_hda_codec_realtek snd_hda_codec_generic snd_hda_codec_hdmi snd_hda_intel kvm_amd kvm snd_hda_controller crct10dif_pclmul snd_hda_codec crc32_pclmul cdc_ether snd_hwdep snd_pcm usbnet ghash_clmulni_intel snd_seq_midi r8152 aesni_intel snd_seq_midi_event snd_rawmidi snd_seq mii snd_seq_device aes_x86_64 lrw snd_timer gf128mul glue_helper joydev edac_core psmouse snd ablk_helper rtsx_pci_ms parport_pc i2c_hid serio_raw cryptd k10temp ppdev edac_mce_amd lp video soundcore rfkill_gpio memstick i2c_designware_platform i2c_piix4 i2c_designware_core parport mac_hid hid_generic usbhid hid rtsx_pci_sdmmc e1000e sdhci_pci sdhci ahci rtsx_pci ptp libahci pps_core
[   86.110674] CPU: 3 PID: 2945 Comm: smep_attack Tainted: G           OE  3.19.0-smep #2
[   86.110945] Hardware name: AMD Gardenia/Gardenia, BIOS TGA1070A 01/28/2015
[   86.111180] task: ffff8800da5d0000 ti: ffff8800da858000 task.ti: ffff8800da858000
[   86.111435] RIP: 0010:[<00000000043ff954>]  [<00000000043ff954>] 0x43ff954
[   86.111677] RSP: 0018:ffff8800da85be88  EFLAGS: 00010292
[   86.111860] RAX: 0000000004000000 RBX: 0000000000000003 RCX: ffff8800da85bf50
[   86.112103] RDX: 0000000000000003 RSI: 0000000000400914 RDI: ffff88010a3c2d00
[   86.112387] RBP: ffff8800da85be88 R08: 0000000000000000 R09: ffff8800db7175a4
[   86.112667] R10: 0000000000000001 R11: 0000000000000246 R12: 0000000000400914
[   86.112910] R13: 0000000000000003 R14: ffff8800da85bf50 R15: 0000000000000000
[   86.113154] FS:  00007f3f56c65740(0000) GS:ffff880111d80000(0000) knlGS:0000000000000000
[   86.113429] CS:  0010 DS: 0000 ES: 0000 CR0: 0000000080050033
[   86.113626] CR2: 00000000043ff954 CR3: 00000000dbf32000 CR4: 00000000000407e0
[   86.113870] Stack:
[   86.113943]  ffff8800da85bea8 ffffffffa0253018 0000000000000001 ffff8800db717540
[   86.114219]  ffff8800da85bed8 ffffffff8124457d 0000000000400914 0000000000000003
[   86.114495]  ffff8800da85bf50 ffff88010a3c2d00 ffff8800da85bf28 ffffffff811de957
[   86.114771] Call Trace:
[   86.114864]  [<ffffffffa0253018>] bug_write+0x18/0x30 [smep_test_driver]
[   86.115099]  [<ffffffff8124457d>] proc_reg_write+0x3d/0x80
[   86.115292]  [<ffffffff811de957>] vfs_write+0xb7/0x1f0
[   86.115470]  [<ffffffff811df566>] SyS_write+0x46/0xb0
[   86.115648]  [<ffffffff81776834>] ? int_check_syscall_exit_work+0x34/0x3d
[   86.115881]  [<ffffffff817765ad>] system_call_fastpath+0x16/0x1b
[   86.116086] Code:  Bad RIP value.
[   86.116253] RIP  [<00000000043ff954>] 0x43ff954
[   86.116429]  RSP <ffff8800da85be88>
[   86.116553] CR2: 00000000043ff954
[   86.116737] ---[ end trace a485173b8df5ad61 ]---


with smep feature support, testlog:

zswan@zswan-developer:~/testapp/smep$ cat withsmep.log
[  617.445096] unable to execute userspace code (SMEP?) (uid: 1000)
[  617.445293] BUG: unable to handle kernel paging request at 0000000004000000
[  617.445521] IP: [<0000000004000000>] 0x4000000
[  617.445696] PGD da443067 PUD e09aa067 PMD dbdf0067 PTE 9d2d3867
[  617.445909] Oops: 0011 [#3] SMP
[  617.446027] Modules linked in: smep_test_driver(OE) rfcomm bnep bluetooth snd_hda_codec_realtek snd_hda_codec_generic kvm_amd snd_hda_codec_hdmi kvm snd_hda_intel snd_hda_controller snd_hda_codec cdc_ether snd_hwdep usbnet snd_pcm crct10dif_pclmul crc32_pclmul snd_seq_midi r8152 snd_seq_midi_event snd_rawmidi ghash_clmulni_intel mii aesni_intel joydev aes_x86_64 snd_seq lrw gf128mul glue_helper ablk_helper parport_pc snd_seq_device psmouse edac_core snd_timer ppdev cryptd lp snd rtsx_pci_ms i2c_hid k10temp serio_raw memstick edac_mce_amd soundcore video rfkill_gpio parport i2c_piix4 i2c_designware_platform mac_hid i2c_designware_core hid_generic rtsx_pci_sdmmc e1000e usbhid hid sdhci_pci ahci ptp rtsx_pci sdhci libahci pps_core [last unloaded: smep_test_driver]
[  617.448497] CPU: 0 PID: 5043 Comm: smep_attack Tainted: G      D    OE  3.19.0-smep #2
[  617.448751] Hardware name: AMD Gardenia/Gardenia, BIOS TGA1070A 01/28/2015
[  617.448961] task: ffff8800dac198e0 ti: ffff8800da580000 task.ti: ffff8800da580000
[  617.449196] RIP: 0010:[<0000000004000000>]  [<0000000004000000>] 0x4000000
[  617.449421] RSP: 0018:ffff8800da583e90  EFLAGS: 00010292
[  617.449584] RAX: 0000000004000000 RBX: 0000000000000003 RCX: ffff8800da583f50
[  617.449800] RDX: 0000000000000003 RSI: 0000000000400914 RDI: ffff8800d9cdb300
[  617.450029] RBP: ffff8800da583ea8 R08: 0000000000000002 R09: ffff8800e04b8124
[  617.450252] R10: 0000000000000003 R11: 0000000000000246 R12: 0000000000400914
[  617.450467] R13: 0000000000000003 R14: ffff8800da583f50 R15: 0000000000000000
[  617.450697] FS:  00007f1e754a8740(0000) GS:ffff880111c00000(0000) knlGS:0000000000000000
[  617.450946] CS:  0010 DS: 0000 ES: 0000 CR0: 0000000080050033
[  617.451121] CR2: 0000000004000000 CR3: 00000000da46e000 CR4: 00000000001407f0
[  617.451348] Stack:
[  617.451418]  ffffffffa0232018 0000000000000001 ffff8800e04b80c0 ffff8800da583ed8
[  617.451672]  ffffffff8124457d 0000000000400914 0000000000000003 ffff8800da583f50
[  617.451920]  ffff8800d9cdb300 ffff8800da583f28 ffffffff811de957 ffff8800da583f08
[  617.452180] Call Trace:
[  617.452275]  [<ffffffffa0232018>] ? bug_write+0x18/0x30 [smep_test_driver]
[  617.452506]  [<ffffffff8124457d>] proc_reg_write+0x3d/0x80
[  617.452693]  [<ffffffff811de957>] vfs_write+0xb7/0x1f0
[  617.452861]  [<ffffffff811df566>] SyS_write+0x46/0xb0
[  617.453026]  [<ffffffff81776834>] ? int_check_syscall_exit_work+0x34/0x3d
[  617.453233]  [<ffffffff817765ad>] system_call_fastpath+0x16/0x1b
[  617.453426] Code:  Bad RIP value.
[  617.453548] RIP  [<0000000004000000>] 0x4000000
[  617.453695]  RSP <ffff8800da583e90>
[  617.453803] CR2: 0000000004000000
[  617.453968] ---[ end trace 27e253e29b53ac23 ]---

