#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "LexActivator.h"

#if _WIN32
#if _WIN64
#pragma comment(lib, "x64/LexActivator")
#else
#pragma comment(lib, "x86/LexActivator")
#endif
#endif

void init()
{
	int status;

#if _WIN32
	// status = SetProductFile(L"ABSOLUTE_PATH_OF_PRODUCT.DAT_FILE");
	status = SetProductData(L"PASTE_CONTENT_OF_PRODUCT.DAT_FILE");
#else
	//  status = SetProductFile("ABSOLUTE_PATH_OF_PRODUCT.DAT_FILE");
	status = SetProductData("PASTE_CONTENT_OF_PRODUCT.DAT_FILE");
#endif
	if (LA_OK != status)
	{
		printf("Error code: %d", status);
		getchar();
		exit(status);
	}

#if _WIN32
	status = SetProductId(L"PASTE_PRODUCT_ID", LA_USER);
#else
	status = SetProductId("PASTE_PRODUCT_VERSION_GUID", LA_SYSTEM);
#endif
	if (LA_OK != status)
	{
		printf("Error code: %d", status);
		getchar();
		exit(status);
	}

#if _WIN32
	status = SetAppVersion(L"2.4.0");
#else
	status = SetAppVersion("2.4.0");
#endif
	if (LA_OK != status)
	{
		printf("Error code: %d", status);
		getchar();
		exit(status);
	}
}

// Ideally on a button click inside a dialog
void activate()
{
	int status;
#if _WIN32
	status = SetLicenseKey(L"PASTE_PRODUCT_KEY");
#else
	status = SetLicenseKey("PASTE_PRODUCT_KEY");
#endif
	if (LA_OK != status)
	{
		printf("Error code: %d", status);
		getchar();
		exit(status);
	}

#if _WIN32
	status = SetActivationMetadata(L"key1", L"value1");
#else
	status = SetActivationMetadata("key1", "value1");
#endif
	if (LA_OK != status)
	{
		printf("Error code: %d", status);
		getchar();
		exit(status);
	}

	status = ActivateLicense();
	if (LA_OK == status || LA_EXPIRED == status || LA_SUSPENDED == status || LA_USAGE_LIMIT_REACHED == status)
	{
		printf("Product activated successfully: %d", status);
	}
	else
	{
		printf("Product activation failed: %d", status);
	}
}

// Ideally on a button click inside a dialog
void activateTrial()
{
	int status;

#if _WIN32
	status = SetTrialActivationMetadata(L"key1", L"value1");
#else
	status = SetTrialActivationMetadata("key1", "value1");
#endif
	if (LA_OK != status)
	{
		printf("Error code: %d", status);
		getchar();
		exit(status);
	}

	status = ActivateTrial();
	if (LA_OK == status)
	{
		printf("Product trial activated successfully!");
	}
	else if (LA_EXPIRED == status)
	{
		printf("Product trial has expired!");
	}
	else
	{
		printf("Product trial activation failed: %d", status);
	}
}

int main()
{
	init();
	int status = IsLicenseGenuine();
	if (LA_OK == status)
	{
		unsigned int expiryDate = 0;
		GetLicenseExpiryDate(&expiryDate);
		int daysLeft = (expiryDate - time(NULL)) / 86500;
		printf("Days left: %d\n", daysLeft);
		printf("Product is genuinely activated!"); 
	}
	else if (LA_EXPIRED == status)
	{
		printf("Product is genuinely activated, but license has expired!");
	}
	else if (LA_SUSPENDED == status)
	{
		printf("Product is genuinely activated, but license has been suspended!");
	}
	else if (LA_USAGE_LIMIT_REACHED == status)
	{
		printf("Product is genuinely activated, but license has reached it's usage limit!");
	}
	else if (LA_GRACE_PERIOD_OVER == status)
	{
		printf("Product is genuinely activated, but grace period is over!");
	}
	else
	{
		int trialStatus;
		trialStatus = IsTrialGenuine();
		if (LA_OK == trialStatus)
		{
			unsigned int trialExpiryDate = 0;
			GetTrialExpiryDate(&trialExpiryDate);
			int daysLeft = (trialExpiryDate - time(NULL)) / 86500;
			printf("Trial days left: %d", daysLeft);
		}
		else if (LA_TRIAL_EXPIRED == trialStatus)
		{
			printf("Trial has expired!");

			// Time to buy the license and activate the app
			activate();
		}
		else
		{
			printf("Either trial has not started or has been tampered!");
			// Activating the trial
			activateTrial();
		}
	}
	getchar();
	return 0;
}
