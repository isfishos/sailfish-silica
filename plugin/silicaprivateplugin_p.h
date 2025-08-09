#ifndef SILICA_PRIVATE_REGISTER_H
#define SILICA_PRIVATE_REGISTER_H

class QQmlEngine;

namespace Silica {
namespace Private {

void registerTypes(const char *uri);
void initializeEngine(QQmlEngine *engine, const char *uri);
void registerPrivateTypes(const char *uri);
void registerBackgroundTypes(const char *uri);

} // namespace Private
} // namespace Silica

#endif // SILICA_PRIVATE_REGISTER_H
