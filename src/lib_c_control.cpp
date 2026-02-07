#include "lib_c_control.h"
#include "flatbuffers/AngleControlMessageBuilder.h"
#include "flatbuffers/RobotConfigurationBuilder.h"
#include "libcontrol.h"
#include "sentry.h"

#undef min
#undef max

#define ACTUATOR_CONTROL_TAG 5
#define TOPOLOGY_CMD_TAG 6
#define MAX_RMT_CHANNELS 4

extern "C" {
const auto robot_controller = std::make_unique<RobotController>();

const auto acm_builder =
    std::make_unique<Flatbuffers::AngleControlMessageBuilder>();

const auto robot_configuration_builder =
    std::make_unique<Flatbuffers::RobotConfigurationBuilder>();

LIB_API void init() {
  spdlog::info("[c_control] Initializing");
  robot_controller->fetchDirectlyConnectedModules(false);
}

LIB_API void cleanup() { spdlog::info("[c_control] Cleanup"); }

LIB_API int send_angle_control(int module_id, int angle) {
  if (const auto maybe_module = robot_controller->getModule(module_id)) {
    const auto module = (*maybe_module).lock();
    module->actuate(angle);
  }
  return 0;
}

LIB_API char *get_configuration(int *size_out) {
  std::vector<Flatbuffers::ModuleInstance> modules_vec{};
  std::vector<Flatbuffers::ModuleConnectionInstance> connections_vec{};

  for (const auto &module : robot_controller->getModuleList()) {
    modules_vec.emplace_back(module);
  }

  for (const auto &connection : robot_controller->getConnections()) {
    connections_vec.emplace_back(connection);
  }

  const auto [data, size] =
      robot_configuration_builder->build_robot_configuration(modules_vec,
                                                             connections_vec);
  *size_out = size;
  return reinterpret_cast<char *>(data);
}

LIB_API bool control_sentry_init(const char *dsn, const char *environment,
                                 const char *release) {
  sentry_options_t *options = sentry_options_new();
  sentry_options_set_dsn(options, dsn);
  sentry_options_set_environment(options, environment);
  sentry_options_set_release(options, release);
  sentry_options_set_auto_session_tracking(options, 1);

  sentry_init(options);
  return true;
}

LIB_API void control_sentry_shutdown(void) { sentry_close(); }

LIB_API void control_sentry_set_app_info(const char *app_name,
                                         const char *app_version,
                                         const char *build_number) {
  sentry_value_t app = sentry_value_new_object();

  if (app_name && *app_name) {
    sentry_value_set_by_key(app, "name", sentry_value_new_string(app_name));
  }

  if (app_version && *app_version) {
    sentry_value_set_by_key(app, "version",
                            sentry_value_new_string(app_version));
  }

  if (build_number && *build_number) {
    sentry_value_set_by_key(app, "build",
                            sentry_value_new_string(build_number));
  }

  sentry_set_context("app", app);

  if (app_version && *app_version) {
    if (build_number && *build_number) {
      char release[256];
      snprintf(release, sizeof(release), "%s@%s+%s",
               app_name && *app_name ? app_name : "app", app_version,
               build_number);
    } else {
      // Example: mygame@1.2.3
      char release[256];
      snprintf(release, sizeof(release), "%s@%s",
               app_name && *app_name ? app_name : "app", app_version);
    }
  }
}
}
