/** \example "Vorb modules initialization"
 * <code><pre>
 * vorb::InitParam modules = vorb::InitParam::GRAPHICS | vorb::InitParam::IO;
 * vorb::InitParam v = vorb::init(modules);
 * if (v != modules) {
 *     puts("Could not initialize all modules");
 *     vorb::dispose(v);
 *     exit(1);
 * }
 * // Do stuff here
 * v = vorb::dispose(modules);
 * if (v != modules) {
 *     puts("Could not dispose all modules");
 *     exit(1);
 * }
 * </pre></code>
 */