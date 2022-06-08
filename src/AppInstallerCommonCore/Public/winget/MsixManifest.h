// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
#pragma once

#include "AppInstallerStrings.h"
#include "AppInstallerVersions.h"

#include <AppxPackaging.h>
#include <wrl/client.h>

namespace AppInstaller::Msix
{
    using string_t = Utility::NormalizedString;
    using PackageVersion = Utility::FourPartsVersionNumber;
    using OSVersion = Utility::FourPartsVersionNumber;

    // Package identity for an MSIX manifest
    struct MsixPackageManifestIdentity
    {
        MsixPackageManifestIdentity(Microsoft::WRL::ComPtr<IAppxManifestPackageId> packageId)
            : m_packageId(packageId) {}

        string_t GetPackageFamilyName() const;
        PackageVersion GetVersion() const;
    private:
        Microsoft::WRL::ComPtr<IAppxManifestPackageId> m_packageId;
    };

    // Target device family for an MSIX manifest
    struct MsixPackageManifestTargetDeviceFamily
    {
        MsixPackageManifestTargetDeviceFamily(Microsoft::WRL::ComPtr<IAppxManifestTargetDeviceFamily> targetDeviceFamily)
            : m_targetDeviceFamily(targetDeviceFamily) {}

        string_t GetName() const;
        OSVersion GetMinVersion() const;
    private:
        Microsoft::WRL::ComPtr<IAppxManifestTargetDeviceFamily> m_targetDeviceFamily;
    };

    // MSIX manifest
    struct MsixPackageManifest
    {
        MsixPackageManifest(Microsoft::WRL::ComPtr<IAppxManifestReader> manifestReader)
            : m_manifestReader(manifestReader) {}

        std::vector<MsixPackageManifestTargetDeviceFamily> GetTargetDeviceFamilies() const;
        MsixPackageManifestIdentity GetIdentity() const;
        std::optional<OSVersion> GetMinimumOSVersion() const;
    private:
        Microsoft::WRL::ComPtr<IAppxManifestReader> m_manifestReader;
    };

    // MSIX manifest cache
    struct MsixPackageManifestCache
    {
        // Construct MSIX manifest or fetch it from cache
        const std::vector<MsixPackageManifest>& GetAppPackageManifests(std::string url);
    private:
        // Map installer url to a vector of msix package manifests
        std::map<std::string, std::vector<MsixPackageManifest>> m_msixManifests;
    };
}