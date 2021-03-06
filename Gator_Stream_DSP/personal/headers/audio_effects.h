//###########################################################################
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//###########################################################################

#ifndef _AUDIO_EFFECTS_H_
#define _AUDIO_EFFECTS_H_
#define MAX_VAL                      (327678.0f * 2.0f)
#define FS_INT                       44100
#define FS_FLOAT                     44100.0f
#define EFFECT_BUFFER_SIZE           2200
#define FLANGER_DELAY_SIZE           132
#define VIBRATO_DELAY_SIZE           309

#define NO_AUDIO_EFFECT              0
#define FLANGER_AUDIO_EFFECT         1
#define OVERDRIVE_AUDIO_EFFECT       2
#define VIBRATO_AUDIO_EFFECT         3
#define WAHWAH_AUDIO_EFFECT          4
#define RINGMODULATION_AUDIO_EFFECT  5
#define CHORUS_AUDIO_EFFECT          6
#define BASSBOOST_AUDIO_EFFECT       7
#define TREBBOOST_AUDIO_EFFECT       8

/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-Enums~-~-~-~--~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
enum side {
  LEFT,
  RIGHT
};

enum buffer {
  FIRST,
  SECOND
};

/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-Function Prototypes-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
/* -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~- */
interrupt void audio_ISR(void);
int16 flangerEffect(float sample, enum side channel);
int16 overDriveEffect(float sample, enum side channel);
int16 vibratoEffect(float sample, enum side channel);
int16 wahwahEffect(float sample, enum side channel);
int16 ringModulationEffect(float sample, enum side channel);
int16 chorusEffect(float sample, enum side channel);
int16 bassBoostEffect(float sample, enum side channel);
int16 trebBoostEffect(float sample, enum side channel);
#endif
