.cpu arm7tdmi
.section "patch_downloadplay7", "ax"
.syntax unified

.thumb

// Entered instead of the child by the child boot function of Download Play. This code is in main
// memory, because Download Play clears the arm7 wram before it starts the child. Waiting in main
// memory would starve the arm9 while it loads Pico Loader: the arm7 has no cache and has main memory
// priority, so a wait loop here keeps the main memory busy. So copy the wait loop to the arm7 wram,
// which Download Play left cleared and unused, and wait there.
.global patch_downloadplay7_entry
.type patch_downloadplay7_entry, %function
patch_downloadplay7_entry:
    adr r0, waitLoop
    adr r1, waitLoopEnd
    ldr r2,= 0x03800000
    movs r3, r2
1:
    ldmia r0!, {r4}
    stmia r2!, {r4}
    cmp r0, r1
    blo 1b
    adds r3, #1 // thumb
    bx r3

// position independent, copied to 0x03800000
.balign 4
waitLoop:
    ldr r0, waitLoopIpcSync
    movs r1, #1
    strb r1, [r0, #1]

2:
    ldrb r7, [r0] // ipc sync
    cmp r7, #1
    bne 2b // while ipc sync from arm9 is not 1

    ldr r0, waitLoopPicoLoader7
    ldr r0, [r0]
    bx r0

.balign 4
waitLoopIpcSync:
    .word 0x04000180

waitLoopPicoLoader7:
    .word 0x06000000
waitLoopEnd:

.pool
.end
