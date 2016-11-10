// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_ENROLLMENT_CONFIG_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_ENROLLMENT_CONFIG_H_

#include <string>

namespace policy {

// A container keeping all parameters relevant to whether and how enterprise
// enrollment of a device should occur. This configures the behavior of the
// enrollment flow during OOBE, i.e. whether the enrollment screen starts
// automatically, whether the user can skip enrollment, and what domain to
// display as owning the device.
struct EnrollmentConfig {
  // Describes the enrollment mode, i.e. what triggered enrollment.
  enum Mode {
    // Enrollment not applicable.
    MODE_NONE,
    // Manually triggered initial enrollment.
    MODE_MANUAL,
    // Manually triggered re-enrollment.
    MODE_MANUAL_REENROLLMENT,
    // Forced enrollment triggered by local OEM manifest or device requisition,
    // user can't skip.
    MODE_LOCAL_FORCED,
    // Advertised enrollment triggered by local OEM manifest or device
    // requisition, user can skip.
    MODE_LOCAL_ADVERTISED,
    // Server-backed-state-triggered forced enrollment, user can't skip.
    MODE_SERVER_FORCED,
    // Server-backed-state-triggered advertised enrollment, user can skip.
    MODE_SERVER_ADVERTISED,
    // Recover from "spontaneous unenrollment", user can't skip.
    MODE_RECOVERY,
    // Start attestation-based enrollment.
    MODE_ATTESTATION,
    // Start attestation-based enrollment and only uses that.
    MODE_ATTESTATION_FORCED,
  };

  // An enumeration of authentication mechanisms that can be used for
  // enrollment.
  enum AuthMechanism {
    // Interactive authentication.
    AUTH_MECHANISM_INTERACTIVE,
    // Automatic authentication relying on the attestation process.
    AUTH_MECHANISM_ATTESTATION,
    // Let the system determine the best mechanism (typically the one
    // that requires the least user interaction).
    AUTH_MECHANISM_BEST_AVAILABLE,
  };

  // Whether enrollment should be triggered.
  bool should_enroll() const {
    return should_enroll_with_attestation() || should_enroll_interactively();
  }

  // Whether attestation enrollment should be triggered.
  bool should_enroll_with_attestation() const {
    return auth_mechanism != AUTH_MECHANISM_INTERACTIVE;
  }

  // Whether interactive enrollment should be triggered.
  bool should_enroll_interactively() const { return mode != MODE_NONE; }

  // Whether enrollment is forced. The user can't skip the enrollment step
  // during OOBE if this returns true.
  bool is_forced() const {
    return mode == MODE_LOCAL_FORCED || mode == MODE_SERVER_FORCED ||
           mode == MODE_RECOVERY || is_attestation_forced();
  }

  // Whether attestation-based enrollment is forced. The user can't skip
  // the enrollment step during OOBE if this returns true.
  bool is_attestation_forced() const {
    return auth_mechanism == AUTH_MECHANISM_ATTESTATION;
  }

  // Whether this configuration is in attestation mode.
  bool is_mode_attestation() const {
    return mode == MODE_ATTESTATION || mode == MODE_ATTESTATION_FORCED;
  }

  // Whether this configuration is in OAuth mode.
  bool is_mode_oauth() const {
    return mode != MODE_NONE && !is_mode_attestation();
  }

  // Indicates the enrollment flow variant to trigger during OOBE.
  Mode mode = MODE_NONE;

  // The domain to enroll the device to, if applicable. If this is not set, the
  // device may be enrolled to any domain. Note that for the case where the
  // device is not already locked to a certain domain, this value is used for
  // display purposes only and the server makes the final decision on which
  // domain the device should be enrolled with. If the device is already locked
  // to a domain, policy validation during enrollment will verify the domains
  // match.
  std::string management_domain;

  // The authentication mechanism to use.
  // TODO(drcrash): Change to best available once ZTE is everywhere.
  AuthMechanism auth_mechanism = AUTH_MECHANISM_INTERACTIVE;
};

}  // namespace policy

#endif  // CHROME_BROWSER_CHROMEOS_POLICY_ENROLLMENT_CONFIG_H_
