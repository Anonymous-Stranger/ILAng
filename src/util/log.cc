/// \file
/// The source file for the logging/asserting system.

#include "util/log.h"
#include "config.h"
#include <set>

namespace ila {

std::ostream* log1_stream = NULL;
std::ostream* log2_stream = NULL;
std::ostream* null_stream = NULL;
std::set<std::string> enabled_tags;

// Macros and handlers for glog-based log system.
/******************************************************************************/

void SetGlogVerboseLevel(const int& lvl) { FLAGS_v = lvl; }

void SetGlogFilePath(const std::string& path) { FLAGS_log_dir = path; }

void SetGlogAlsoToStdErr(const int& b) { FLAGS_alsologtostderr = b; }

void InitGlog(const int& lvl, const std::string& path, const int& also) {
  SetGlogVerboseLevel(lvl);
  SetGlogFilePath(path);
  SetGlogAlsoToStdErr(also);
  google::InitGoogleLogging("ila_log");
}

void CloseGlog() {}

// Wrapper for debug log system.
/******************************************************************************/

void ClearStream(std::ostream*& ptr) {
  // Clear previous specified log file.
  if (ptr != &std::cout) {
    delete ptr;
  }
  ptr = new std::ofstream("/dev/null");
}

void SetStream(std::ostream*& ptr, const std::string& filename) {
  // Clear previous specified log file.
  if (ptr != &std::cout) {
    delete ptr;
  }
  // If no file name specified, default use standard output.
  if (filename == "") {
    ptr = &std::cout;
  } else {
    ptr = new std::ofstream(filename);
  }
}

void InitDlog() {
  log1_stream = new std::ofstream("/dev/null");
  log2_stream = new std::ofstream("/dev/null");
  null_stream = new std::ofstream("/dev/null");
}

void CloseDlog() {
  ClearStream(log1_stream);
  ClearStream(log2_stream);
  ClearStream(null_stream);
}

void SetDlogLevel(int l, const std::string& filename) {
  if (l == 0) {
    // No debug logging.
    ClearStream(log1_stream);
    ClearStream(log2_stream);
  } else if (l == 1) {
    // Channel 1 turned on.
    SetStream(log1_stream, filename);
    ClearStream(log2_stream);
  } else if (l >= 2) {
    // Channel 1 & 2 turned on.
    SetStream(log1_stream, filename);
    log2_stream = log1_stream;
  }
}

void EnableDlog(const std::string& tag) { enabled_tags.insert(tag); }

void DisableDlog(const std::string& tag) { enabled_tags.erase(tag); }

void ClearDlogs() { enabled_tags.clear(); }

std::ostream& IlaDlog(std::ostream& l, const std::string& tag) {
  ILA_WARN_IF(l == NULL) << "Debug log stream not initialized.\n";
  if (l == NULL) {
    InitDlog();
  }

#ifdef DEBUG
  if (enabled_tags.find(tag) != enabled_tags.end()) {
    l << "[" << tag << "]";
    return l.flush();
  }
#endif // DEBUG
  return *null_stream;
}

std::ostream& IlaDlog1(const std::string& tag) {
  return IlaDlog(*log1_stream, tag);
}

std::ostream& IlaDlog2(const std::string& tag) {
  return IlaDlog(*log2_stream, tag);
}

} // namespace ila

