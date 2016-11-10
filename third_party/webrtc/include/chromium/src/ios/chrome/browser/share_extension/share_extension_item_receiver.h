// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IOS_CHROME_BROWSER_UI_SHARE_EXTENSION_SHARE_EXTENSION_ITEM_RECEIVER_H_
#define IOS_CHROME_BROWSER_UI_SHARE_EXTENSION_SHARE_EXTENSION_ITEM_RECEIVER_H_

#import <Foundation/Foundation.h>

namespace bookmarks {
class BookmarkModel;
}

class ReadingListModel;

// This class observes the Application group folder
// |app_group::ShareExtensionItemsFolder()| and process the files it contains
// when a new file is created or when application is put in foreground.
@interface ShareExtensionItemReceiver : NSObject

+ (instancetype)sharedInstance;

// Sets the bookmark and reading list models to use. |shutdown| must be called
// before other models are set.
// The receiver will start observe the share extension folder and send items to
// these models.
- (void)setBookmarkModel:(bookmarks::BookmarkModel*)bookmarkModel
        readingListModel:(ReadingListModel*)readingListModel;

// Stops observers and pending operations.
- (void)shutdown;

@end

#endif  // IOS_CHROME_BROWSER_UI_SHARE_EXTENSION_SHARE_EXTENSION_ITEM_RECEIVER_H_
