//  Plugins Specifications for GAMECUBE emulators.
//  Version 1.0 (for plugin developers).
//
//  VERSION HISTORY :
//  -----------------
//
//  0    21/10/04 org
//  Specs are created. Now only GX, AX, PAD and DVD plugins, but NET also planned.
//

// do not modify this file without major reason !
// you should report Dolwin team, about all changes :
// org <kvzorganic@mail.ru>, hotquik <hotquik@hotmail.com>

#ifndef __DOLWIN_PLUGIN_SPECS_H__
#define __DOLWIN_PLUGIN_SPECS_H__

// return values are always 1 for good, and 0 for bad.

// plugin specification version
#define DOL_PLUG_VER            "1.0"

// plugin types
// plugin may be multi-purposed (types can be ORed)
#define DOL_PLUG_GX             (0x01)
#define DOL_PLUG_AX             (0x02)
#define DOL_PLUG_PAD            (0x04)
#define DOL_PLUG_DVD            (0x08)
#define DOL_PLUG_NET            (0x10)

// plugin type detection
#define IS_DOL_PLUG_GX(type)    (type & DOL_PLUG_GX)
#define IS_DOL_PLUG_AX(type)    (type & DOL_PLUG_AX)
#define IS_DOL_PLUG_PAD(type)   (type & DOL_PLUG_PAD)
#define IS_DOL_PLUG_DVD(type)   (type & DOL_PLUG_DVD)
#define IS_DOL_PLUG_NET(type)   (type & DOL_PLUG_NET)

// plugin data. should be passed to RegisterPlugin as parameter
typedef struct
{
    // these values are passed as parameters during plugin init
    void*           display;    // handler of drawing device (HWND* in Windows, for example)
    unsigned char*  ram;        // your 24 MB memory buffer

    // these values are returned by plugin
    unsigned long   type;       // see DOL_PLUG_* types
    char*           version;    // plugin description and version string
} PluginData;

// used to verify plugin
void RegisterPlugin(PluginData * plugData);

// ---------------------------------------------------------------------------

// GX plugin (3D graphics)

// GXOpen() should be called before emulation started, to initialize
// plugin. GXClose() is called, when emulation is stopped, to shutdown plugin.
long GXOpen();
void GXClose();

// add new data to graphics fifo. draw next primitive, if there are enough data.
// should be called, when CPU is writing any data into 0x0C008000, when CP is
// working in "linked" mode ("single-buffer" fifo). emulator should take care
// about "multi-buffer" fifo mode itself.
void GXWriteFifo(unsigned char *dataPtr, unsigned long length);

// set pointers to GX tokens.
// when fifo reaches "draw done", it will set peDrawDone to 1.
// when fifo reaches token, it will set peToken to 1, and tokenVal to token value.
// screen is updated automatically, after DrawDone or Token event.
// GX doesnt responsible for clearing token values (should be performed by emu).
void GXSetTokens(long *peDrawDone, long *peToken, unsigned short *tokenVal);

// config / about
void GXConfigure();
void GXAbout();

// savestate (flag:0 to load, flag:1 to save);
// "filename" is currently running file name.
void GXSaveLoad(long flag, char *filename);

// ---------------------------------------------------------------------------

// AX plugin (audio)

// AXOpen() should be called before emulation started, to initialize
// plugin. AXClose() is called, when emulation is stopped, to shutdown plugin.
long AXOpen();
void AXClose();

// play audio buffer (little-endian PCM, 16-bit, 44 khz). count in bytes.
void AXPlayBuffer(unsigned short *buf, long count);

// user stuff
void AXConfigure();
void AXAbout();

// savestate (flag:0 to load, flag:1 to save)
// "filename" is currently running file name.
void AXSaveLoad(long flag, unsigned char *filename);

// ---------------------------------------------------------------------------

// PAD plugin (input)
// (padnum = 0...3)

#pragma pack(1)     // align structure for byte
typedef struct
{
    unsigned short  button;         // combination of PAD_BUTTON*
    signed char     stickX;         // -127...127 NOT clamped
    signed char     stickY;
    signed char     substickX;      // -127...127 NOT clamped
    signed char     substickY;
    unsigned char   triggerLeft;    // 0...255 NOT clamped
    unsigned char   triggerRight;
} PADState;
#pragma pack()      // restore old value

// controller buttons mask
#define PAD_BUTTON_LEFT         (0x0001)
#define PAD_BUTTON_RIGHT        (0x0002)
#define PAD_BUTTON_DOWN         (0x0004)
#define PAD_BUTTON_UP           (0x0008)
#define PAD_TRIGGER_Z           (0x0010)
#define PAD_TRIGGER_R           (0x0020)
#define PAD_TRIGGER_L           (0x0040)
#define PAD_BUTTON_A            (0x0100)
#define PAD_BUTTON_B            (0x0200)
#define PAD_BUTTON_X            (0x0400)
#define PAD_BUTTON_Y            (0x0800)
#define PAD_BUTTON_START        (0x1000)

// motor control command
#define PAD_MOTOR_STOP          0
#define PAD_MOTOR_RUMBLE        1
#define PAD_MOTOR_STOP_HARD     2

// PADOpen() should be called before emulation started, to initialize
// plugin. PADClose() is called, when emulation is stopped, to shutdown plugin.
long PADOpen();
void PADClose();

// read controller buttons state. returns 1, if ok, and 0, if PAD not connected
long PADReadButtons(long padnum, PADState *state);

// controller motor. 0 returned, if rumble is not supported by PAD.
// see one of PAD_MOTOR* for allowed commands.
long PADSetRumble(long padnum, unsigned long cmd);

// config / about
void PADConfigure(long padnum);
void PADAbout();

// savestate (flag:0 to load, flag:1 to save)
// "filename" is currently running file name.
void PADSaveLoad(long flag, unsigned char *filename);

// ---------------------------------------------------------------------------

// DVD plugin

// DVDOpen() should be called before emulation started, to initialize
// plugin. DVDClose() is called, when emulation is stopped, to shutdown plugin.
long DVDOpen();
void DVDClose();

// set current DVD image for read/seek/open file operations
// return 1 if no errors, and 0 if cannot open file
long DVDSetCurrent(char *file);

// return 1, if DVD image is compressed
long DVDIsCompressed(char *file);

// seek and read operations on current DVD
void DVDSeek(long position);
void DVDRead(void *buffer, long length);

// open file in DVD root. return file position, or 0 if no such file.
// note : current DVD must be selected first!
// example use : s32 banner = DVDOpenFile("/opening.bnr");
long DVDOpenFile(char *dvdfile);

// config / about
void DVDConfigure();
void DVDAbout();

// savestate (flag:0 to load, flag:1 to save)
// "filename" is currently running file name.
void DVDSaveLoad(long flag, unsigned char *filename);

#endif  // __DOLWIN_PLUGIN_SPECS_H__

// EOF
