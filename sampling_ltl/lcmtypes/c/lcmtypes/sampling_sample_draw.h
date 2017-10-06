// THIS IS AN AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY
// BY HAND!!
//
// Generated by lcm-gen

#ifndef _sampling_sample_draw_h
#define _sampling_sample_draw_h

#include <stdint.h>
#include <stdlib.h>
#include <lcm/lcm_coretypes.h>
#include <lcm/lcm.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _sampling_sample_draw sampling_sample_draw;
struct _sampling_sample_draw
{
    int8_t     if_draw;
};

/**
 * Create a deep copy of a sampling_sample_draw.
 * When no longer needed, destroy it with sampling_sample_draw_destroy()
 */
sampling_sample_draw* sampling_sample_draw_copy(const sampling_sample_draw* to_copy);

/**
 * Destroy an instance of sampling_sample_draw created by sampling_sample_draw_copy()
 */
void sampling_sample_draw_destroy(sampling_sample_draw* to_destroy);

/**
 * Identifies a single subscription.  This is an opaque data type.
 */
typedef struct _sampling_sample_draw_subscription_t sampling_sample_draw_subscription_t;

/**
 * Prototype for a callback function invoked when a message of type
 * sampling_sample_draw is received.
 */
typedef void(*sampling_sample_draw_handler_t)(const lcm_recv_buf_t *rbuf,
             const char *channel, const sampling_sample_draw *msg, void *userdata);

/**
 * Publish a message of type sampling_sample_draw using LCM.
 *
 * @param lcm The LCM instance to publish with.
 * @param channel The channel to publish on.
 * @param msg The message to publish.
 * @return 0 on success, <0 on error.  Success means LCM has transferred
 * responsibility of the message data to the OS.
 */
int sampling_sample_draw_publish(lcm_t *lcm, const char *channel, const sampling_sample_draw *msg);

/**
 * Subscribe to messages of type sampling_sample_draw using LCM.
 *
 * @param lcm The LCM instance to subscribe with.
 * @param channel The channel to subscribe to.
 * @param handler The callback function invoked by LCM when a message is received.
 *                This function is invoked by LCM during calls to lcm_handle() and
 *                lcm_handle_timeout().
 * @param userdata An opaque pointer passed to @p handler when it is invoked.
 * @return 0 on success, <0 if an error occured
 */
sampling_sample_draw_subscription_t* sampling_sample_draw_subscribe(lcm_t *lcm, const char *channel, sampling_sample_draw_handler_t handler, void *userdata);

/**
 * Removes and destroys a subscription created by sampling_sample_draw_subscribe()
 */
int sampling_sample_draw_unsubscribe(lcm_t *lcm, sampling_sample_draw_subscription_t* hid);

/**
 * Sets the queue capacity for a subscription.
 * Some LCM providers (e.g., the default multicast provider) are implemented
 * using a background receive thread that constantly revceives messages from
 * the network.  As these messages are received, they are buffered on
 * per-subscription queues until dispatched by lcm_handle().  This function
 * how many messages are queued before dropping messages.
 *
 * @param subs the subscription to modify.
 * @param num_messages The maximum number of messages to queue
 *  on the subscription.
 * @return 0 on success, <0 if an error occured
 */
int sampling_sample_draw_subscription_set_queue_capacity(sampling_sample_draw_subscription_t* subs,
                              int num_messages);

/**
 * Encode a message of type sampling_sample_draw into binary form.
 *
 * @param buf The output buffer.
 * @param offset Encoding starts at this byte offset into @p buf.
 * @param maxlen Maximum number of bytes to write.  This should generally
 *               be equal to sampling_sample_draw_encoded_size().
 * @param msg The message to encode.
 * @return The number of bytes encoded, or <0 if an error occured.
 */
int sampling_sample_draw_encode(void *buf, int offset, int maxlen, const sampling_sample_draw *p);

/**
 * Decode a message of type sampling_sample_draw from binary form.
 * When decoding messages containing strings or variable-length arrays, this
 * function may allocate memory.  When finished with the decoded message,
 * release allocated resources with sampling_sample_draw_decode_cleanup().
 *
 * @param buf The buffer containing the encoded message
 * @param offset The byte offset into @p buf where the encoded message starts.
 * @param maxlen The maximum number of bytes to read while decoding.
 * @param msg Output parameter where the decoded message is stored
 * @return The number of bytes decoded, or <0 if an error occured.
 */
int sampling_sample_draw_decode(const void *buf, int offset, int maxlen, sampling_sample_draw *msg);

/**
 * Release resources allocated by sampling_sample_draw_decode()
 * @return 0
 */
int sampling_sample_draw_decode_cleanup(sampling_sample_draw *p);

/**
 * Check how many bytes are required to encode a message of type sampling_sample_draw
 */
int sampling_sample_draw_encoded_size(const sampling_sample_draw *p);

// LCM support functions. Users should not call these
int64_t __sampling_sample_draw_get_hash(void);
uint64_t __sampling_sample_draw_hash_recursive(const __lcm_hash_ptr *p);
int __sampling_sample_draw_encode_array(void *buf, int offset, int maxlen, const sampling_sample_draw *p, int elements);
int __sampling_sample_draw_decode_array(const void *buf, int offset, int maxlen, sampling_sample_draw *p, int elements);
int __sampling_sample_draw_decode_array_cleanup(sampling_sample_draw *p, int elements);
int __sampling_sample_draw_encoded_array_size(const sampling_sample_draw *p, int elements);
int __sampling_sample_draw_clone_array(const sampling_sample_draw *p, sampling_sample_draw *q, int elements);

#ifdef __cplusplus
}
#endif

#endif
