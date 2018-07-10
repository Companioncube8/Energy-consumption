#include "energylib.h"


int energy_consuption(double *energy_return, int count)
{
	unsigned int deviceCount = 0;
	nvmlReturn_t nvmlResult;
	unsigned int energy = 0, error;
	nvmlDevice_t nvmlDeviceID;
	nvmlPciInfo_t nvmPCIInfo;
	nvmlEnableState_t pmmode;
	nvmlComputeMode_t computeMode;
	char deviceName[64];
	*energy_return = 0;
	nvmlResult = nvmlInit();
	if (NVML_SUCCESS != nvmlResult)
	{
		printf("NVML Init fail: %s\n", nvmlErrorString(nvmlResult));
		error = code_errors (nvmlResult);
		return error;
	}
	
	nvmlResult = nvmlDeviceGetCount(&deviceCount);
	if (NVML_SUCCESS != nvmlResult)
	{
		printf("Failed to query device count: %s\n", nvmlErrorString(nvmlResult));
		error = code_errors (nvmlResult);
		return error;
	}

	for (int i = 0; i < deviceCount; i++)
	{
		if (i <= count){
			nvmlResult = nvmlDeviceGetHandleByIndex(i, &nvmlDeviceID);
			if (NVML_SUCCESS != nvmlResult)
			{
				printf("Failed to get handle for device %d: %s\n", i, nvmlErrorString(nvmlResult));
				error = code_errors (nvmlResult);
				return error;
			}

			nvmlResult = nvmlDeviceGetName(nvmlDeviceID, deviceName, sizeof(deviceName)/sizeof(deviceName[0]));
			if (NVML_SUCCESS != nvmlResult)
			{
				printf("Failed to get name of device %d: %s\n", i, nvmlErrorString(nvmlResult));
				error = code_errors (nvmlResult);
				return error;
			}

			nvmlResult = nvmlDeviceGetPciInfo(nvmlDeviceID, &nvmPCIInfo);
			if (NVML_SUCCESS != nvmlResult)
			{
				printf("Failed to get PCI info of device %d: %s\n", i, nvmlErrorString(nvmlResult));
				error = code_errors (nvmlResult);
				return error;
			}

			nvmlResult = nvmlDeviceGetComputeMode(nvmlDeviceID, &computeMode);
			if (NVML_ERROR_NOT_SUPPORTED == nvmlResult)
			{
				printf("This is not a CUDA-capable device.\n");
				return 3;
			} else if (NVML_SUCCESS != nvmlResult)
			{
				printf("Failed to get compute mode for device %i: %s\n", i, nvmlErrorString(nvmlResult));
				error = code_errors (nvmlResult);
				return error;
			}
			
			nvmlResult = nvmlDeviceGetPowerManagementMode(nvmlDeviceID, &pmmode);
		
			if (pmmode == NVML_FEATURE_ENABLED)
			{
				//unsigned int min, max;
				//nvmlDeviceGetPowerManagementLimitConstraints(nvmlDeviceID, &min, &max);
				//printf("%d, %d\n", min, max);
				//nvmlResult = nvmlDeviceSetPowerManagementLimit(nvmlDeviceID, 60000);
				//nvmlResult = nvmlDeviceSetPowerManagementLimit(nvmlDeviceID, 141000);
				//if (NVML_SUCCESS != nvmlResult)
				//{
				//	error = code_errors (nvmlResult);
				//	printf("%d\n", error);
				//	return error;
				//}
				nvmlResult = nvmlDeviceGetPowerUsage(nvmlDeviceID, &energy);
				if (nvmlResult == NVML_SUCCESS){
						if (count != 1){
							energy_return[i] = (double)energy/1000;
						}else{
							*energy_return = (double)energy/1000;
						}
				} else{
					printf("Failed to get energy consumption for device %i: %s\n", i, nvmlErrorString(nvmlResult));
					error = code_errors (nvmlResult);
					return error;
				}
			}
		}
	}
	return 0;
}

int code_errors (nvmlReturn_t nvmlResult)
{
	switch(nvmlResult) {
		case NVML_SUCCESS:
			return 0;
		case NVML_ERROR_UNINITIALIZED:
			return 1;
		case NVML_ERROR_INVALID_ARGUMENT:
			return 2;
		case NVML_ERROR_NOT_SUPPORTED:
			return 3;
		case NVML_ERROR_NO_PERMISSION:
			return 4;
		case NVML_ERROR_ALREADY_INITIALIZED:
			return 5;
		case NVML_ERROR_NOT_FOUND:
			return 6;
		case NVML_ERROR_INSUFFICIENT_SIZE:
			return 7;
		case NVML_ERROR_INSUFFICIENT_POWER:
			return 8;
		case NVML_ERROR_DRIVER_NOT_LOADED:
			return 9;	
		case NVML_ERROR_TIMEOUT:
			return 10;	
		case NVML_ERROR_IRQ_ISSUE:
			return 11;	
		case NVML_ERROR_LIBRARY_NOT_FOUND:
			return 12;	
		case NVML_ERROR_FUNCTION_NOT_FOUND:
			return 13;	
		case NVML_ERROR_CORRUPTED_INFOROM:
			return 14;
		case NVML_ERROR_GPU_IS_LOST:
			return 15;	
		case NVML_ERROR_RESET_REQUIRED:
			return 16;	
		case NVML_ERROR_OPERATING_SYSTEM:
			return 17;	
		case NVML_ERROR_LIB_RM_VERSION_MISMATCH:
			return 18;	
		case NVML_ERROR_IN_USE:
			return 19;	
		case NVML_ERROR_MEMORY:
			return 20;	
		case NVML_ERROR_NO_DATA:
			return 21;	
		case NVML_ERROR_VGPU_ECC_NOT_SUPPORTED:
			return 22;	
		case NVML_ERROR_UNKNOWN:
			return 999;	
	}
}
