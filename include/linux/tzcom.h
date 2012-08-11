/* Qualcomm TrustZone communicator API */

#ifndef __TZCOM_H_
#define __TZCOM_H_

#include <linux/types.h>
#include <linux/ioctl.h>

/**
 * struct tzcom_register_svc_op_req - for register service ioctl request
 * @svc_id - service id (shared between userspace and TZ)
 * @cmd_id_low - low number in cmd_id range (shared between userspace and TZ)
 * @cmd_id_high - high number in cmd_id range (shared between userspace and TZ)
 * @instance_id - unique id for the given service generated by tzcom driver
 */
struct tzcom_register_svc_op_req {
	uint32_t svc_id; /* in */
	uint32_t cmd_id_low; /* in */
	uint32_t cmd_id_high; /* in */
	uint32_t instance_id; /* out */
};

/**
 * struct tzcom_unregister_svc_op_req - for unregister service ioctl request
 * @svc_id - service id to unregister (provided in register_service request)
 * @instance_id - instance id generated in register service request
 */
struct tzcom_unregister_svc_op_req {
	uint32_t svc_id; /* in */
	uint32_t instance_id; /* in */
};

/**
 * struct tzcom_next_cmd_op_req - for read next command ioctl request
 * @svc_id - has to be a registered svc_id (see @tzcom_register_svc_op_req)
 * @instance_id - unique id for the given service (see @tzcom_register_svc_op_req)
 * @cmd_id - command to execute on the given service, received from TZ
 * @req_len - request buffer length, received from TZ
 * @req - request buffer, received from TZ
 */
struct tzcom_next_cmd_op_req {
	uint32_t svc_id; /* in */
	uint32_t instance_id; /* in */
	uint32_t cmd_id; /* out */
	unsigned int req_len; /* in/out */
	void *req_buf; /* in/out */
};

/**
 * struct tzcom_send_cmd_op_req - for send command ioctl request
 * @cmd_id - command to execute on TZBSP side
 * @cmd_len - command buffer length
 * @cmd_buf - command buffer
 * @resp_len - response buffer length
 * @resp_buf - response buffer
 */
struct tzcom_send_cmd_op_req {
	uint32_t cmd_id; /* in */
	unsigned int cmd_len; /* in */
	void *cmd_buf; /* in */
	unsigned int resp_len; /* in/out */
	void *resp_buf; /* in/out */
};
#if 0
static int __tzcom_is_cont_cmd(struct tzcom_data_t *data)
{
	int ret;
	ret = (data->cont_cmd_flag != 0);
	return ret || data->abort;
}
#endif
/**
 * struct tzcom_cont_cmd_op_req - for continue command ioctl request. used
 * as a trigger from HLOS service to notify TZCOM that it's done with its
 * operation and provide the response for TZCOM can continue the incomplete
 * command execution
 * @cmd_id - Command to continue filled in by tzcom as tzcom knows about the
 *           last incomplete command.
 * @instance_id - Instance id of the svc
 * @resp_len - Length of the response
 * @resp_buf - Response buffer where the response of the cmd should go.
 */
struct tzcom_cont_cmd_op_req {
	uint32_t cmd_id; /* out */
	uint32_t instance_id; /* in */
	unsigned int resp_len; /* in */
	void *resp_buf; /* in */
};

#define TZCOM_IOC_MAGIC    0x97

/* For HLOS service */
#define TZCOM_IOCTL_REGISTER_SERVICE_REQ \
	_IOWR(TZCOM_IOC_MAGIC, 1, struct tzcom_register_svc_op_req)
/* For HLOS service */
#define TZCOM_IOCTL_UNREGISTER_SERVICE_REQ \
	_IOWR(TZCOM_IOC_MAGIC, 2, struct tzcom_unregister_svc_op_req)
/* For TZ service */
#define TZCOM_IOCTL_SEND_CMD_REQ \
	_IOWR(TZCOM_IOC_MAGIC, 3, struct tzcom_send_cmd_op_req)
/* For HLOS service */
#define TZCOM_IOCTL_READ_NEXT_CMD_REQ \
	_IOWR(TZCOM_IOC_MAGIC, 4, struct tzcom_next_cmd_op_req)
/* For TZ service */
#define TZCOM_IOCTL_CONTINUE_CMD_REQ \
	_IOWR(TZCOM_IOC_MAGIC, 5, struct tzcom_cont_cmd_op_req)

#define TZCOM_IOCTL_ABORT_REQ _IO(TZCOM_IOC_MAGIC, 6)

#endif /* __TZCOM_H_ */
