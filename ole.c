#include <stdio.h>

#include <vapi/vapi.h>
#include <vapi/tapv2.api.vapi.h>

DEFINE_VAPI_MSG_IDS_TAPV2_API_JSON

vapi_ctx_t g_vapi_ctx = NULL;

#define UNUSED(x) (void)x

static vapi_error_e
tap_delete_v2_cb(vapi_ctx_t ctx, void *caller_ctx, vapi_error_e rv,
                 bool is_last, vapi_payload_tap_delete_v2_reply * reply)
{
    UNUSED(ctx); UNUSED(caller_ctx); UNUSED(rv); UNUSED(is_last);
    return reply->retval;
}

static vapi_error_e bin_api_delete_tapv2(u32 sw_if_index)
{
    vapi_msg_tap_delete_v2 *mp;
    vapi_error_e rv;

    mp = vapi_alloc_tap_delete_v2(g_vapi_ctx);
    if (!mp)
        return VAPI_ENOMEM;

    mp->payload.sw_if_index = sw_if_index;

    while ( (rv = vapi_tap_delete_v2(g_vapi_ctx, mp, tap_delete_v2_cb, NULL))
            == VAPI_EAGAIN ) {
        rv = vapi_dispatch(g_vapi_ctx);
    }
    if (rv != VAPI_OK)
        return -rv;

    return VAPI_OK;
}

int main(int argc, char **argv)
{
    vapi_error_e rv;

    if (argc != 2) {
        fprintf(stderr, "Wrong input. Use ./remove_tap_by_sw_index index\n");
        goto cleanup;
    }

    printf("Removing sw_if_index %s\n", argv[1]);

    /* Connect to VPP API */
    rv = vapi_ctx_alloc(&g_vapi_ctx);
    rv = vapi_connect(g_vapi_ctx, "app_ole", NULL, 4, 2, VAPI_MODE_BLOCKING, true);
    if (rv != VAPI_OK) {
        vapi_ctx_free(g_vapi_ctx);
        fprintf(stderr, "Problem connecting to VPP API\n");
        return -1;
    }

    bin_api_delete_tapv2(atoi(argv[1]));

cleanup:
    /* Disconnect from VPP API */
    if (g_vapi_ctx)
        vapi_disconnect(g_vapi_ctx);

    return 0;
}
