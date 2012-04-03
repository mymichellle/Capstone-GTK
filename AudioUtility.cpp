/* 
 * AudioUtility.cpp
 *
 *
 */

#include "AudioUtility.h"
#include "portaudio.h"
#define SOUND_DEBUG

using namespace std;

AudioUtility::AudioUtility()
{


}

void AudioUtility::recordSound(char* fileName)
{
    PaStreamParameters inputParameters;
    PaStream *stream;
    PaError err;
    SAMPLE *recordedSamples;
    int i;
    int totalFrames;
    int numSamples;
    int numBytes;
    SAMPLE max, average, val;

#ifdef SOUND_DEBUG
    printf("Initializing sound recording.\n"); fflush(stdout);
#endif

    /* Allocate a buffer for recorded data */
    totalFrames = NUM_SECONDS * SAMPLE_RATE;
    numSamples = totalFrames * NUM_CHANNELS;

    numBytes = numSamples * sizeof(SAMPLE);
    recordedSamples = (SAMPLE*) malloc( numBytes );
    if( recordedSamples == NULL)
    {
        printf("Could not allocate record array.\n");
        exit(1);
    }
    for (i=0; i<numSamples; i++)
    {
        recordedSamples[i] = 0;
    }

    /* Initialize PortAudio */
    err = Pa_Initialize();
    if (err != paNoError)
    {
        printf("Error Initializing Port Audio");
        Pa_Terminate();
        break;
    }

#ifdef SOUND_DEBUG
    PaDeviceIndex numDevices = Pa_GetDeviceCount();
    printf("Number of available devices: %d\n", numDevices);

    for (i=0; i < numDevices; i++) 
    {
      PaDeviceInfo *pid = (PaDeviceInfo *) Pa_GetDeviceInfo(i);
      printf("struct version: %d\n", pid->structVersion);
      printf("name: %s\n", pid->name);
      printf("maxInputChannels: %d\n", pid->maxInputChannels);
      printf("maxOutputChannels: %d\n", pid->maxOutputChannels);
    }

    printf("Default input device index: %d", Pa_GetDefaultInputDevice());
    printf("Default output device index: %d", Pa_GetDefaultOutputDevice());
#endif

    /* Assign an input device and set input parameters */
    inputParameters.device = Pa_GetDefaultInputDevice(); 
    if (inputParameters.device == paNoDevice)
    {
        fprintf(stderr, "Error: No default input device.\n");
        Pa_Terminate();
        break;
        //goto error;
    }
    inputParameters.channelCount = NUM_CHANNELS;
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    /* Record Audio */
    err = Pa_OpenStream(
        &stream,
        &inputParameters,
        NULL,   /* OutputParameters */
        SAMPLE_RATE,
        FRAMES_PER_BUFFER,
        paClipOff,
        NULL,   /* no callback */
        NULL ); /* no callback, no callback userData */

    if (err != paNoError )
    {
        printf("Error: Pa_OpenStream()");
        Pa_Terminate();
        break;
    }

#ifdef SOUND_DEBUG
    printf("Now recording!!\n"); fflush(stdout);
#endif

    err = Pa_ReadStream( stream, recordedSamples, totalFrames );
    if( err != paNoError )
    {
        printf("Error: Pa_ReadStream()");
        Pa_Terminate();
        break;
    };
    
    err = Pa_CloseStream( stream );
    if( err != paNoError )
    {
        printf("Error: Pa_CloseStream()");
        Pa_Terminate();
        break;
    }

    /* Measure maximum peak amplitude. */
    max = 0;
    average = 0;
    for( i=0; i<numSamples; i++ )
    {
        val = recordedSamples[i];
        if( val < 0 ) val = -val; /* ABS */
        if( val > max )
        {
            max = val;
        }
        average += val;
    }

    average = average / numSamples;

#ifdef SOUND_DEBUG
    printf("Sample max amplitude = "PRINTF_S_FORMAT"\n", max );
    printf("Sample average = "PRINTF_S_FORMAT"\n", average );
#endif

    /* Write recorded data to a file. */
    {
        FILE  *fid;
        fid = fopen(fileName, "wb");
        if( fid == NULL )
        {
            printf("Could not open file.");
        }
        else
        {
            fwrite( recordedSamples, NUM_CHANNELS * sizeof(SAMPLE), totalFrames, fid );
            fclose( fid );

#ifdef SOUND_DEBUG
            printf("Wrote data to '%s'\n", fileName);
#endif

        }
    }

    free( recordedSamples );
    Pa_Terminate();

error:
Pa_Terminate();
#ifdef SOUND_DEBUG
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
#endif
}

void AudioUtility::playbackSound(char* fileName)
{
    PaStreamParameters outputParameters;
    PaStream *stream;
    PaError err;
    SAMPLE *recordedSamples;
    int i;
    int totalFrames;
    int numSamples;
    int numBytes;

    err = Pa_Initialize();
    if (err != paNoError) {}//goto error;

    totalFrames = NUM_SECONDS * SAMPLE_RATE; /* Determine length of recording */
    numSamples = totalFrames * NUM_CHANNELS;

    numBytes = numSamples * sizeof(SAMPLE);
    recordedSamples = (SAMPLE *) malloc( numBytes );

    /* Read recorded data from a file. */
    {
        FILE  *fid;
        fid = fopen(fileName, "rb");
        if( fid == NULL )
        {
            printf("Could not open file.");
        }
        else
        {
            fread( recordedSamples, NUM_CHANNELS * sizeof(SAMPLE), totalFrames, fid );
            fclose( fid );

#ifdef SOUND_DEBUG
            printf("Read data from '%s'\n", fileName);
#endif

        }
    }

    outputParameters.device = Pa_GetDefaultOutputDevice();
    if (outputParameters.device == paNoDevice)
    {

#ifdef SOUND_DEBUG
        fprintf(stderr, "Error: No default output device.\n");
#endif

        //goto error;
    }
    outputParameters.channelCount = NUM_CHANNELS;
    outputParameters.sampleFormat = PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
        &stream,
        NULL, /* no input */
        &outputParameters,
        SAMPLE_RATE,
        FRAMES_PER_BUFFER,
        paClipOff, 
        NULL, /* no callback, using blocking API */
        NULL ); /* no callback, so no callback userData */
    if (err != paNoError) {}//goto error;
    if ( stream )
    {
        err = Pa_StartStream( stream );
        if (err != paNoError) {}//goto error;}

        err = Pa_WriteStream( stream, recordedSamples, totalFrames );
        if (err != paNoError) {}//goto error;}
        
        err = Pa_CloseStream( stream );
        if (err != paNoError) {}//goto error;}
    }
    free (recordedSamples);

    Pa_Terminate();

error:
Pa_Terminate();
#ifdef SOUND_DEBUG
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
#endif
}
