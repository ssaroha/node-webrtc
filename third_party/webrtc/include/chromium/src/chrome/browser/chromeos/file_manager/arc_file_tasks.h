// Copyright (c) 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_FILE_MANAGER_ARC_FILE_TASKS_H_
#define CHROME_BROWSER_CHROMEOS_FILE_MANAGER_ARC_FILE_TASKS_H_

#include <memory>
#include <string>
#include <vector>

#include "chrome/browser/chromeos/file_manager/file_tasks.h"

class Profile;

namespace extensions {
struct EntryInfo;
}

namespace storage {
class FileSystemURL;
}

namespace file_manager {
namespace file_tasks {

// Finds the ARC tasks that can handle |entries|, appends them to |result_list|,
// and calls back to |callback|.
void FindArcTasks(Profile* profile,
                  const std::vector<extensions::EntryInfo>& entries,
                  std::unique_ptr<std::vector<FullTaskDescriptor>> result_list,
                  const FindTasksCallback& callback);

// Executes the specified task by ARC.
bool ExecuteArcTask(Profile* profile,
                    const TaskDescriptor& task,
                    const std::vector<storage::FileSystemURL>& file_urls,
                    const std::vector<std::string>& mime_types);

}  // namespace file_tasks
}  // namespace file_manager

#endif  // CHROME_BROWSER_CHROMEOS_FILE_MANAGER_ARC_FILE_TASKS_H_
